# Ground Station API

The API runs with Python's standard library:

```powershell
.\run-satellite-gui.ps1
```

Open `http://127.0.0.1:8765`.

The same API also has a no-dependency Node.js runner:

```powershell
cd satellite
npm run start
```

Run the Node API tests:

```powershell
cd satellite
npm test
```

## Endpoints

`GET /api/state`

Returns the complete current simulation state, health report, storage counters, and event records.

`GET /api/events`

Returns the recent event log.

`GET /api/commands`

Returns accepted commands with labels, groups, and descriptions.

`GET /api/status`

Returns service health and endpoint metadata.

`POST /api/command`

Request body:

```json
{ "command": "science" }
```

Applies the command, advances one mission tick, and returns the updated state.

`POST /api/script`

Request body:

```json
{ "script": "nominal\nscience\nstep\ndownlink" }
```

Runs each non-empty, non-comment command line and returns all command results plus final state.

`POST /api/reset`

Resets the simulation state.
