import gc
from types import SimpleNamespace
import weakref

import pytest

from openpilot.selfdrive.ui.mici.onroad import cameraview as mici_cameraview
from openpilot.selfdrive.ui.onroad import cameraview as big_cameraview


@pytest.mark.parametrize("module", (mici_cameraview, big_cameraview))
def test_road_transition_releases_camera_buffers(monkeypatch, module):
  class FakeClient:
    pass

  view = module.CameraView.__new__(module.CameraView)
  old_client = FakeClient()
  old_client_ref = weakref.ref(old_client)
  view._name = "camerad"
  view._stream_type = object()
  view.client = old_client
  view.frame = None
  view.available_streams = [object()]
  view._target_client = FakeClient()
  view._target_stream_type = object()
  view._switching = True
  view._texture_needs_update = False
  view.last_connection_attempt = 123.0
  view._closed = True
  cleared = []
  view._clear_textures = lambda: cleared.append(True)

  monkeypatch.setattr(module, "VisionIpcClient", lambda *_args, **_kwargs: FakeClient())
  del old_client

  view._offroad_transition()
  gc.collect()

  assert old_client_ref() is None
  assert cleared == [True]
  assert view.frame is None
  assert view.available_streams == []
  assert view._target_client is None
  assert view._target_stream_type is None
  assert view._switching is False
  assert view._texture_needs_update
  assert view.last_connection_attempt == 0.0


@pytest.mark.parametrize("module", (mici_cameraview, big_cameraview))
def test_transition_callback_does_not_retain_camera_view(monkeypatch, module):
  class FakeClient:
    pass

  callbacks = []
  monkeypatch.setattr(module, "TICI", False)
  monkeypatch.setattr(module, "VisionIpcClient", lambda *_args, **_kwargs: FakeClient())
  monkeypatch.setattr(module.rl, "load_shader_from_memory", lambda *_args: SimpleNamespace(id=1))
  monkeypatch.setattr(module.rl, "get_shader_location", lambda *_args: 0)
  monkeypatch.setattr(module.rl, "unload_shader", lambda *_args: None)
  monkeypatch.setattr(module.ui_state, "add_offroad_transition_callback", callbacks.append)
  monkeypatch.setattr(module.ui_state, "remove_offroad_transition_callback", callbacks.remove)

  view = module.CameraView("camerad", object())
  view_ref = weakref.ref(view)
  assert len(callbacks) == 1

  del view
  gc.collect()

  assert view_ref() is None
  assert callbacks == []
