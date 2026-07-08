from __future__ import annotations

import json
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from typing import Any
from urllib.parse import urlparse

from .simulation import SatelliteSimulator, VALID_COMMANDS


ROOT = Path(__file__).resolve().parent
STATIC_ROOT = ROOT / "static"
simulator = SatelliteSimulator()


class GroundStationHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args: Any, **kwargs: Any) -> None:
        super().__init__(*args, directory=str(STATIC_ROOT), **kwargs)

    def do_GET(self) -> None:
        path = urlparse(self.path).path
        if path == "/api/state":
            self._send_json(simulator.snapshot())
            return
        if path == "/api/events":
            self._send_json({"events": simulator.events()})
            return
        if path == "/api/commands":
            self._send_json({"commands": sorted(VALID_COMMANDS)})
            return
        if path == "/":
            self.path = "/index.html"
        super().do_GET()

    def do_POST(self) -> None:
        path = urlparse(self.path).path
        if path == "/api/command":
            body = self._read_json()
            command = str(body.get("command", ""))
            self._send_json(simulator.command(command))
            return
        if path == "/api/reset":
            self._send_json(simulator.reset())
            return
        self.send_error(404, "Unknown API endpoint")

    def end_headers(self) -> None:
        self.send_header("Cache-Control", "no-store")
        super().end_headers()

    def _read_json(self) -> dict[str, Any]:
        length = int(self.headers.get("Content-Length", "0"))
        raw = self.rfile.read(length) if length > 0 else b"{}"
        try:
            value = json.loads(raw.decode("utf-8"))
            return value if isinstance(value, dict) else {}
        except json.JSONDecodeError:
            return {}

    def _send_json(self, payload: dict[str, Any]) -> None:
        encoded = json.dumps(payload, indent=2).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(encoded)))
        self.end_headers()
        self.wfile.write(encoded)


def run(host: str = "127.0.0.1", port: int = 8765) -> None:
    server = ThreadingHTTPServer((host, port), GroundStationHandler)
    print(f"Asteria ground station running at http://{host}:{port}")
    server.serve_forever()


if __name__ == "__main__":
    run()

