# SmartDisaster — Frontend Completion Phase

This package is the frontend-only checkpoint for the SmartDisaster DAA project.

## What is included

- Dashboard
- City Map & Network
- Emergencies
- Evacuation
- Rescue Teams
- Shelters
- Resources
- Algorithm Laboratory (Unit I–VI catalog + Run Demo UI)
- Disaster Simulation
- Reports & Analytics

## Frontend-only behavior

All pages use deterministic local/mock data. No Flask or C++ process is required for this phase.
The service layer is kept separate so the same UI can later be connected to Flask/C++ without redesigning the pages.

The existing evacuation API file is retained for the later backend phase, but the current Evacuation page uses a local preview so the frontend can be tested standalone.

## Important frontend interactions

- City Map: BFS/DFS selection, start location, traversal display, level/step view, map highlighting.
- Emergencies: search, severity filter, affected-people sorting, details dialog, add local incident.
- Evacuation: source/destination/criteria selectors, route calculation, route steps, route visualization.
- Rescue Teams: availability filter and local team-to-emergency assignment.
- Shelters: occupancy/available capacity, distribution chart, details dialog.
- Resources: inventory filtering, fractional-knapsack loading preview, coin-change preview.
- Algorithm Lab: complete Unit I–VI list, search/filter, details, local demo output.
- Simulation: scenario configuration, reset, animated decision pipeline and response summary.
- Reports: charts and JSON export.

## Run locally

From the `frontend/frontend` folder:

```cmd
npm run dev
```

The frontend can be tested without starting Flask during this phase.

## Later backend phase

The next phase will replace the local service implementations with:

React -> Flask API -> C++ algorithms -> JSON -> React

The first integrated backend feature already proved in the project is Dijkstra evacuation routing; it will be restored when the backend phase begins.
