import hashlib
import sys
from datetime import datetime, timezone
from pathlib import Path

import pytest

MODULE_DIR = Path(__file__).resolve().parents[1]
if str(MODULE_DIR) not in sys.path:
  sys.path.insert(0, str(MODULE_DIR))

import webtransport  # noqa: E402

REPO_ROOT = Path(__file__).resolve().parents[4]
GALAXY_PY = MODULE_DIR / "the_galaxy.py"
MOBILE_ROOT = MODULE_DIR / "assets" / "mobile"


def _x509():
  from cryptography import x509
  return x509


def _load_cert(path):
  from cryptography.x509 import load_pem_x509_certificate
  return load_pem_x509_certificate(Path(path).read_bytes())


def _utc(value):
  return value if value.tzinfo else value.replace(tzinfo=timezone.utc)


def test_generated_certificate_is_pinnable(tmp_path):
  from cryptography.hazmat.primitives import serialization
  from cryptography.hazmat.primitives.asymmetric import ec

  info = webtransport.ensure_server_certificate(
    tmp_path,
    hostnames=["galaxy.local"],
    ips=["192.168.1.50"],
  )

  cert = _load_cert(info["certPath"])
  assert isinstance(cert.public_key(), ec.EllipticCurvePublicKey)
  assert cert.public_key().curve.name == "secp256r1"

  not_before = _utc(cert.not_valid_before_utc if hasattr(cert, "not_valid_before_utc") else cert.not_valid_before)
  not_after = _utc(cert.not_valid_after_utc if hasattr(cert, "not_valid_after_utc") else cert.not_valid_after)
  assert (not_after - not_before).total_seconds() <= 14 * 24 * 3600

  der = cert.public_bytes(serialization.Encoding.DER)
  assert info["fingerprint"] == hashlib.sha256(der).hexdigest()
  assert len(info["fingerprint"]) == 64
  assert len(info["spkiFingerprint"]) == 64

  san = cert.extensions.get_extension_for_class(_x509().SubjectAlternativeName).value
  assert any(isinstance(name, _x509().IPAddress) for name in san)


def test_certificate_persists_and_rotates(tmp_path):
  first = webtransport.ensure_server_certificate(tmp_path, ips=["10.0.0.2"])
  assert first["rotated"] is True

  second = webtransport.ensure_server_certificate(tmp_path, ips=["10.0.0.2"])
  assert second["rotated"] is False
  assert second["fingerprint"] == first["fingerprint"]

  forced = webtransport.ensure_server_certificate(tmp_path, ips=["10.0.0.2"], force=True)
  assert forced["rotated"] is True
  assert forced["fingerprint"] != first["fingerprint"]


def test_certificate_rotation_happens_before_expiry(tmp_path):
  generated = webtransport.ensure_server_certificate(tmp_path)
  rotated = webtransport.ensure_server_certificate(
    tmp_path,
    now=datetime.now(timezone.utc) + webtransport.CERT_RENEW_AFTER,
  )
  assert rotated["rotated"] is True
  assert rotated["fingerprint"] != generated["fingerprint"]


def test_certificate_files_are_private(tmp_path):
  import os

  info = webtransport.ensure_server_certificate(tmp_path)
  assert os.stat(info["keyPath"]).st_mode & 0o777 == 0o600
  assert os.stat(info["certPath"]).st_mode & 0o777 == 0o600


def test_certificate_hashes_shape(tmp_path):
  info = webtransport.ensure_server_certificate(tmp_path)
  hashes = webtransport.certificate_hashes(info["fingerprint"])
  assert hashes == [{"algorithm": "sha-256", "value": info["fingerprint"]}]


def test_availability_is_a_boolean():
  assert isinstance(webtransport.is_available(), bool)


def test_server_status_without_aioquic(tmp_path):
  if webtransport.is_available():
    pytest.skip("aioquic present; unavailable path is not exercised")

  server = webtransport.WebTransportServer(tmp_path)
  assert server.start() is False
  assert server.error == "aioquic is not installed"
  assert server.status()["available"] is False
  assert server.status()["running"] is False


def test_webtransport_api_routes_are_registered():
  source = GALAXY_PY.read_text(encoding="utf-8")
  assert '@app.route("/api/webtransport/status", methods=["GET"])' in source
  assert '@app.route("/api/webtransport/certificate", methods=["GET"])' in source
  assert '@app.route("/api/webtransport/restart", methods=["POST"])' in source


def test_mobile_client_uses_certificate_pinning():
  client = (MOBILE_ROOT / "js" / "webtransport.js").read_text(encoding="utf-8")
  assert "new WebTransport(" in client
  assert "serverCertificateHashes" in client
  assert "buildServerCertificateHashes" in client
  assert 'algorithm: "sha-256"' in client
  assert "createUnidirectionalStream" in client
  assert "incomingUnidirectionalStreams" in client
  assert "isSecureContext" in client
  # Feature-detect the API instead of gating on a browser User-Agent: pinning is
  # available in Chromium and Firefox 125+.
  assert "navigator.userAgent" not in client
  assert '"WebTransport" in window && window.isSecureContext' in client


def test_local_probe_skips_mixed_content_on_https():
  api = (MOBILE_ROOT / "js" / "api.js").read_text(encoding="utf-8")
  # An HTTPS Galaxy page must not fetch the plain-HTTP LAN origin; Chromium
  # blocks it as mixed active content.
  assert 'window.location?.protocol === "https:"' in api
  assert 'fetch(`http://${host}:8082/`' in api


def test_webtransport_tunnel_supports_streaming_responses():
  # SSE endpoints such as /api/routes must stream incrementally; buffering the
  # whole body made the Recordings list stick at 0%.
  source = (MODULE_DIR / "webtransport.py").read_text(encoding="utf-8")
  assert "streaming" in source
  assert "_next_chunk" in source
  assert "_STREAM_END" in source

  galaxy = GALAXY_PY.read_text(encoding="utf-8")
  assert "response.is_streamed" in galaxy
  assert "stream_payload" in galaxy

  client = (MOBILE_ROOT / "js" / "webtransport.js").read_text(encoding="utf-8")
  assert "new ReadableStream" in client
  assert "header.streaming" in client
  assert "controller.enqueue" in client


def test_mobile_client_logs_transport_diagnostics():
  client = (MOBILE_ROOT / "js" / "webtransport.js").read_text(encoding="utf-8")
  api = (MOBILE_ROOT / "js" / "api.js").read_text(encoding="utf-8")
  assert "__GALAXY_WT_DEBUG" in client
  assert "[Galaxy:WT]" in client
  assert "[Galaxy:API]" in api
  assert "getRoutesStream: chunk" in api


def test_route_player_streams_media_progressively():
  api = (MOBILE_ROOT / "js" / "api.js").read_text(encoding="utf-8")
  recordings = (MOBILE_ROOT / "js" / "views" / "Recordings.js").read_text(encoding="utf-8")
  assert "fetchMediaStream" in api
  assert "MediaSource" in recordings
  assert "addSourceBuffer" in recordings
  assert "appendBuffer" in recordings
  assert "playMse" in recordings
  assert "playSegmentBlob" in recordings
  utilities = (MODULE_DIR / "utilities.py").read_text(encoding="utf-8")
  assert "ffmpeg_stream_segment_mp4" in utilities
  galaxy = GALAXY_PY.read_text(encoding="utf-8")
  assert 'request.args.get("stream") == "1"' in galaxy


def test_vendored_aioquic_is_importable():
  if not webtransport.is_available():
    pytest.skip("vendored aioquic is not available on this architecture")
  import aioquic  # noqa: F401
  import pylsqpack  # noqa: F401


def _free_udp_port():
  import socket

  with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
    sock.bind(("127.0.0.1", 0))
    return sock.getsockname()[1]


def test_server_status_advertises_pin(tmp_path):
  if not webtransport.is_available():
    pytest.skip("aioquic is not vendored for this architecture")
  info = webtransport.ensure_server_certificate(tmp_path, ips=["127.0.0.1"])
  assert info["fingerprint"]
  assert webtransport.certificate_hashes(info["fingerprint"])[0]["algorithm"] == "sha-256"


@pytest.mark.skipif(not webtransport.is_available(), reason="aioquic is not vendored for this architecture")
def test_webtransport_http_tunnel_proxies_requests(tmp_path):
  import asyncio
  import json
  import ssl

  from aioquic.asyncio import connect, QuicConnectionProtocol
  from aioquic.h3.connection import H3Connection
  from aioquic.h3.events import HeadersReceived, WebTransportStreamDataReceived
  from aioquic.quic.configuration import QuicConfiguration
  from aioquic.quic.events import ProtocolNegotiated

  def fake_handler(method, path, headers, body):
    payload = json.dumps({
      "method": method,
      "path": path,
      "accept": headers.get("accept", ""),
      "body": body.decode("utf-8") if body else "",
    }).encode("utf-8")
    return 200, {"content-type": "application/json", "x-path": path}, payload

  class Client(QuicConnectionProtocol):
    def __init__(self, *args, **kwargs):
      super().__init__(*args, **kwargs)
      self.http = None
      self.session_id = None
      self.responses = {}

    def quic_event_received(self, event):
      if isinstance(event, ProtocolNegotiated):
        self.http = H3Connection(self._quic, enable_webtransport=True)
      if self.http is None:
        return
      for http_event in self.http.handle_event(event):
        if isinstance(http_event, HeadersReceived):
          for name, value in http_event.headers:
            if name == b":status":
              self.session_id = http_event.stream_id
        elif isinstance(http_event, WebTransportStreamDataReceived):
          self.responses.setdefault(http_event.stream_id, bytearray()).extend(http_event.data)

  port = _free_udp_port()
  server = webtransport.WebTransportServer(
    tmp_path,
    host="127.0.0.1",
    port=port,
    http_handler=fake_handler,
  )
  assert server.start(), server.error
  try:
    async def run_client():
      configuration = QuicConfiguration(
        is_client=True,
        alpn_protocols=["h3"],
        verify_mode=ssl.CERT_NONE,
        max_datagram_frame_size=65536,
      )
      async with connect("127.0.0.1", port, configuration=configuration, create_protocol=Client) as client:
        await client.wait_connected()
        session = client._quic.get_next_available_stream_id()
        client.http.send_headers(session, [
          (b":method", b"CONNECT"), (b":scheme", b"https"),
          (b":authority", b"localhost"), (b":path", b"/"),
          (b":protocol", b"webtransport"),
        ], end_stream=False)
        client.transmit()
        for _ in range(200):
          if client.session_id is not None:
            break
          await asyncio.sleep(0.02)
        assert client.session_id is not None

        request_stream = client.http.create_webtransport_stream(client.session_id, is_unidirectional=True)
        request = json.dumps({
          "type": "http", "id": 7, "method": "GET",
          "path": "/api/routes?x=1", "headers": {"accept": "application/json"},
        })
        client._quic.send_stream_data(request_stream, (request + "\n").encode(), end_stream=True)
        client.transmit()

        response = None
        for _ in range(200):
          for stream_id, buffer in list(client.responses.items()):
            if stream_id == request_stream:
              continue
            newline = buffer.find(b"\n")
            if newline == -1:
              continue
            header = json.loads(bytes(buffer[:newline]))
            if header.get("id") == 7:
              response = (header, bytes(buffer[newline + 1:]))
              break
          if response:
            break
          await asyncio.sleep(0.02)
        assert response is not None, "no tunnel response"
        header, body = response
        assert header["status"] == 200
        assert header["headers"]["x-path"] == "/api/routes?x=1"
        assert json.loads(body)["path"] == "/api/routes?x=1"
        assert json.loads(body)["method"] == "GET"

    asyncio.run(run_client())
  finally:
    server.stop()


def test_mobile_client_exposes_http_tunnel():
  client = (MOBILE_ROOT / "js" / "webtransport.js").read_text(encoding="utf-8")
  assert "class WebTransportLocalClient" in client
  assert 'type: "http"' in client
  assert "ensureSharedLocalClient" in client
  assert "createUnidirectionalStream" in client
  assert "incomingUnidirectionalStreams" in client

  api = (MOBILE_ROOT / "js" / "api.js").read_text(encoding="utf-8")
  assert "setLocalTransport" in api
  assert "transportFetch" in api
  assert "resolveMediaUrl" in api

  recordings = (MOBILE_ROOT / "js" / "views" / "Recordings.js").read_text(encoding="utf-8")
  assert "ensureSharedLocalClient" in recordings
  assert "resolveMediaUrl" in recordings
  assert "recPlaySrc" in recordings


def test_galaxy_wires_flask_tunnel_handler():
  source = GALAXY_PY.read_text(encoding="utf-8")
  assert "_handle_webtransport_request" in source
  assert "http_handler=_handle_webtransport_request" in source
  assert "_GALAXY_FLASK_APP = app" in source
  assert "test_client()" in source


def test_mobile_media_never_falls_back_to_galaxy_relay():
  # Recordings must only ever play through the direct device WebTransport
  # tunnel. When that tunnel is down, media requests must fail instead of
  # silently streaming the dashcam video through the Galaxy/VPS origin.
  api = (MOBILE_ROOT / "js" / "api.js").read_text(encoding="utf-8")
  assert "LocalTransportUnavailableError" in api
  assert "requireLocalTransport" in api
  assert "transportLabel" in api
  assert "WebTransport socket" in api
  assert "no socket" in api
  assert "VPS relay origin" in api

  recordings = (MOBILE_ROOT / "js" / "views" / "Recordings.js").read_text(encoding="utf-8")
  assert "ensureLocalTransport" in recordings
  assert "requireLocalTransport: this.onFirestar" in recordings
  assert "protect your bandwidth" in recordings


