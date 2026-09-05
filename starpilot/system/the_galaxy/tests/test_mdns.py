import importlib.util
import socket
import struct
import sys

from pathlib import Path

from test_dashboard_stats import MODULE_DIR, _install_server_import_stubs

NAME = "openpilot.starpilot.common.favorite_slots"
PREVIOUS = sys.modules.get(NAME)


def _load_galaxy_module():
  _install_server_import_stubs()
  try:
    spec = importlib.util.spec_from_file_location("galaxy_mdns_server", MODULE_DIR / "the_galaxy.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module
  finally:
    if PREVIOUS is None:
      sys.modules.pop(NAME, None)
    else:
      sys.modules[NAME] = PREVIOUS


galaxy = _load_galaxy_module()


def _build_query(hostname):
  query = bytearray()
  for label in hostname.split("."):
    if label:
      query.append(len(label))
      query += label.encode("ascii")
  query.append(0)
  query += struct.pack(">HH", 1, 1)
  return struct.pack(">HHHHHH", 0, 0, 0, 0, 0, 1) + bytes(query)


def test_hostname_is_galaxy_local():
  assert galaxy.GALAXY_HOSTNAME == "galaxy.local"


def test_returns_a_qtype_for_galaxy_local_query():
  assert galaxy._mdns_query_qtype(_build_query("galaxy.local")) == 1


def test_returns_none_for_unrelated_queries():
  assert galaxy._mdns_query_qtype(_build_query("ThePond.local")) is None
  assert galaxy._mdns_query_qtype(_build_query("foo.galaxy.local")) is None


def test_returns_none_for_responses():
  query = bytearray(_build_query("galaxy.local"))
  query[2] |= 0x80
  assert galaxy._mdns_query_qtype(bytes(query)) is None


def test_build_a_response_is_well_formed():
  ip = "192.168.1.50"
  response = galaxy._mdns_a_response(ip)
  transaction_id, flags, qdcount, ancount, nscount, arcount = struct.unpack(">HHHHHH", response[:12])
  assert transaction_id == 0
  assert flags == 0x8400
  assert ancount == 1
  assert galaxy._mdns_encode(galaxy.GALAXY_HOSTNAME).lower() in response.lower()
  assert response[-4:] == socket.inet_aton(ip)


def test_build_aaaa_response_is_well_formed():
  ipv6 = "2601:405:4500:5630:20a:f5ff:fe3d:ec83"
  response = galaxy._mdns_aaaa_response(ipv6)
  transaction_id, flags, qdcount, ancount, nscount, arcount = struct.unpack(">HHHHHH", response[:12])
  assert ancount == 1
  assert response[-16:] == socket.inet_pton(socket.AF_INET6, ipv6)
