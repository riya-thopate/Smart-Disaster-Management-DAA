import { useEffect, useMemo, useState, type FormEvent } from 'react';
import {
  Activity,
  AlertTriangle,
  Ambulance,
  ArrowRight,
  ArrowUpRight,
  BarChart3,
  BedDouble,
  Binary,
  CheckCircle2,
  ChevronRight,
  Clock3,
  Download,
  Flame,
  Gauge,
  House,
  MapPinned,
  Navigation as RouteIcon,
  Play,
  Plus,
  RotateCcw,
  Search,
  Share2,
  ShieldCheck,
  Siren,
  SlidersHorizontal,
  Users,
  Utensils,
  Waves,
  XCircle,
  Droplets,
  Pill as PillIcon,
} from 'lucide-react';
import { Link } from '@tanstack/react-router';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { API_BASE_URL } from '@/lib/evacuationApi';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import {
  locations,
  roads,
  emergencies as initialEmergencies,
  shelters,
  teams as initialTeams,
  resources,
  resourceTotals,
  algorithms,
  activeAlgorithms,
  previewRoute,
  type Emergency,
  type Algorithm,
} from '@/data/mock';
import {
  Panel,
  Pill,
  SectionHeading,
  Stat,
  DataTable,
  CapacityBar,
  MockLabel,
} from '@/components/disaster/kit';
import { CityGraph } from '@/components/disaster/CityGraph';
import {
  runBFS,
  runDFS,
  runMST,
  runFractionalKnapsack,
  runCoinChange,
  runMaxFlowDemo,
  runBipartiteMatchingDemo,
  runTspDemo,
  routeForIncident,
} from '@/lib/localAlgorithms';
import {
  BarChart,
  Bar,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
  AreaChart,
  Area,
  PieChart,
  Pie,
  Cell,
  Legend,
} from 'recharts';

const severityTone = (
  s: string
): 'critical' | 'warning' | 'success' | 'info' | 'neutral' =>
  s === 'Critical'
    ? 'critical'
    : s === 'High'
      ? 'warning'
      : s === 'Active'
        ? 'critical'
        : s === 'Resolved'
          ? 'success'
          : 'neutral';

function EmergencyIcon({ type }: { type: string }) {
  return type === 'Fire' ? (
    <Flame size={15} />
  ) : type === 'Flood' ? (
    <Waves size={15} />
  ) : (
    <Ambulance size={15} />
  );
}

function EmergencyTable({
  rows = initialEmergencies,
  withActions = false,
  onSelect,
}: {
  rows?: Emergency[];
  withActions?: boolean;
  onSelect?: (e: Emergency) => void;
}) {
  return (
    <DataTable
      headers={[
        'ID',
        'TYPE',
        'LOCATION',
        'SEVERITY',
        'AFFECTED',
        'RESOURCE',
        'STATUS',
        ...(withActions ? ['ACTION'] : []),
      ]}
      rows={rows.map((e) => [
        <strong key={`${e.id}-id`}>{e.id}</strong>,
        <span className="cell-icon" key={`${e.id}-type`}>
          <EmergencyIcon type={e.type} /> {e.type.toUpperCase()}
        </span>,
        e.location,
        <Pill key={`${e.id}-sev`} tone={severityTone(e.severity)}>
          {e.severity.toUpperCase()}
        </Pill>,
        e.affected.toLocaleString(),
        e.resource.toUpperCase(),
        <Pill
          key={`${e.id}-status`}
          tone={e.status === 'Resolved' ? 'success' : 'critical'}
        >
          {e.status.toUpperCase()}
        </Pill>,
        ...(withActions
          ? [
              <Button
                key={`${e.id}-view`}
                variant="link"
                size="sm"
                onClick={() => onSelect?.(e)}
              >
                View details <ChevronRight size={14} />
              </Button>,
            ]
          : []),
      ])}
    />
  );
}

function ShelterCards({
  detailed = false,
  onSelect,
  rows = shelters,
}: {
  detailed?: boolean;
  onSelect?: (id: string) => void;
  rows?: typeof shelters;
}) {
  return (
    <div className={detailed ? 'shelter-grid' : 'shelter-list'}>
    {rows.map((s) => {
        const available = s.capacity - s.occupied;
        const occupancy = (s.occupied / s.capacity) * 100;

        return (
          <button
            key={s.id}
            className="shelter-item"
            onClick={() => onSelect?.(s.id)}
            type="button"
          >
            <div className="shelter-title">
              <div className="shelter-icon">
                <House size={17} />
              </div>
              <div>
                <strong>{s.name}</strong>
                <span>
                  {detailed
                    ? `${s.id} · ${s.locationId}`
                    : `${s.capacity} total capacity`}
                </span>
              </div>
              <Pill tone="success">OPEN</Pill>
            </div>

            <div className="shelter-stats">
              <span>
                <b>{s.occupied}</b> occupied
              </span>
              <span>
                <b>{available}</b> available
              </span>
            </div>

            <CapacityBar
              value={occupancy}
              tone={occupancy >= 70 ? 'warning' : 'success'}
            />

            <div className="shelter-bottom">
              <span>Occupancy</span>
              <strong>{occupancy.toFixed(1)}%</strong>
            </div>
          </button>
        );
      })}
    </div>
  );
}

function ResourceSummary({
  liveResources,
}: {
  liveResources?: Record<string, number | undefined>;
}) {
  const iconMap = [Droplets, Utensils, PillIcon, BedDouble];
  const resourceKeys = ['WATER', 'FOOD', 'MEDICINE', 'BLANKET'];
  const resourceUnits = ['LITERS', 'PACKETS', 'PACKETS', 'UNITS'];

  return (
    <div className="resource-summary">
      {resourceTotals.map((resource, index) => {
       const Icon = iconMap[index] ?? Droplets;
const resourceKey = resourceKeys[index];
const resourceUnit = resourceUnits[index] ?? '';

const quantity = resourceKey
  ? liveResources?.[resourceKey]
  : undefined;

const value =
  typeof quantity === 'number'
    ? `${quantity.toLocaleString()} ${resourceUnit}`
    : resource.value;

        return (
          <div className="resource-summary-item" key={resource.name}>
            <span className="resource-icon">
              <Icon size={18} />
            </span>
            <div>
              <span>{resource.name}</span>
              <strong>{value}</strong>
            </div>
          </div>
        );
      })}
    </div>
  );
}

type DashboardApiResponse = {
  success: boolean;
  emergencies: {
    active: number;
    people_affected: number;
    total: number;
  };
  rescue_teams: {
    available: number;
    total: number;
  };
  shelters: {
    available: number;
    occupied: number;
    total_capacity: number;
  };
  resources: Record<string, number>;
};

export function Dashboard() {
  const [dashboardData, setDashboardData] =
    useState<DashboardApiResponse | null>(null);

  useEffect(() => {
    let active = true;

    async function loadDashboard() {
      try {
        const response = await fetch(`${API_BASE_URL}/dashboard`);

        if (!response.ok) {
          throw new Error(`Dashboard API error: ${response.status}`);
        }

        const data =
          (await response.json()) as DashboardApiResponse;

        if (data.success !== true) {
          throw new Error(
            'The dashboard API returned an unsuccessful response.'
          );
        }

        if (active) {
          setDashboardData(data);
        }
      } catch (error) {
        console.error('Failed to load dashboard data:', error);
      }
    }

    void loadDashboard();

    return () => {
      active = false;
    };
  }, []);

  // Keep the previous mock values as a fallback if the API is unavailable.
  const activeEmergencies =
    dashboardData?.emergencies.active ??
    initialEmergencies.length;

  const peopleAffected =
    dashboardData?.emergencies.people_affected ??
    initialEmergencies.reduce(
      (total, emergency) => total + emergency.affected,
      0
    );

  const availableTeams =
    dashboardData?.rescue_teams.available ??
    initialTeams.length;

  const totalShelter =
    dashboardData?.shelters.total_capacity ??
    shelters.reduce((total, shelter) => total + shelter.capacity, 0);

  const occupiedShelter =
    dashboardData?.shelters.occupied ??
    shelters.reduce((total, shelter) => total + shelter.occupied, 0);

  const availableShelter =
    dashboardData?.shelters.available ??
    totalShelter - occupiedShelter;

  return (
    <>
      <SectionHeading
        eyebrow="OPERATIONS / OVERVIEW"
        title="Emergency Operations Dashboard"
        description="Real-time disaster monitoring and DAA-based optimization"
        action={<MockLabel />}
      />

      <div className="stats-grid">
        <Stat
          icon={AlertTriangle}
          label="Active Emergencies"
          value={String(activeEmergencies)}
          note="Requires attention"
          tone="critical"
        />

        <Stat
          icon={Users}
          label="People Affected"
          value={peopleAffected.toLocaleString()}
          note="Across active incidents"
          tone="warning"
        />

        <Stat
          icon={ShieldCheck}
          label="Available Rescue Teams"
          value={String(availableTeams)}
          note="All currently ready"
          tone="success"
        />

        <Stat
          icon={House}
          label="Shelter Capacity"
          value={`${occupiedShelter.toLocaleString()} / ${totalShelter.toLocaleString()}`}
          note={`${availableShelter.toLocaleString()} spaces available`}
          tone="info"
        />
      </div>

      <div className="dashboard-grid">
        <Panel
          title="Active Emergencies"
          className="dashboard-emergencies"
          action={
            <Link className="panel-link" to="/emergencies">
              View all <ArrowUpRight size={15} />
            </Link>
          }
        >
          <EmergencyTable />
        </Panel>

        <Panel
          title="City Network Overview"
          className="dashboard-map"
          action={
            <Link className="panel-link" to="/city-map">
              Explore map <ArrowUpRight size={15} />
            </Link>
          }
        >
          <CityGraph compact />
        </Panel>

        <Panel
          title="Shelter Capacity"
          action={
            <Link className="panel-link" to="/shelters">
              View shelters <ArrowUpRight size={15} />
            </Link>
          }
        >
          <ShelterCards />
        </Panel>

        <Panel
          title="Resource Status"
          action={
            <Link className="panel-link" to="/resources">
              Inventory <ArrowUpRight size={15} />
            </Link>
          }
        >
          <ResourceSummary liveResources={dashboardData?.resources} />
        </Panel>

        <Panel
          title="Algorithm Status"
          className="algorithm-status"
          action={
            <Link className="panel-link" to="/algorithms">
              Algorithm lab <ArrowUpRight size={15} />
            </Link>
          }
        >
          <div className="algorithm-chip-grid">
            {activeAlgorithms.map((algorithm) => (
              <div className="algorithm-chip" key={algorithm}>
                <span>
                  <Binary size={15} />
                  {algorithm}
                </span>
                <Pill tone="success">READY</Pill>
              </div>
            ))}
          </div>
        </Panel>
      </div>
    </>
  );
}
type TraversalApiResponse = {
  success: boolean;
  algorithm: string;
  traversal: string[];
levels?: number[];
  visited_count: number;
  time_complexity: string;
  space_complexity: string;
};

type CityTraversalResult = {
  algorithm: string;
  order: string[];
  levels: number[];
};
export function CityMapPage() {
  const [source, setSource] = useState('L10');
  const [algorithm, setAlgorithm] = useState<'BFS' | 'DFS'>('BFS');
  const [result, setResult] =
  useState<CityTraversalResult | null>(null);

const [showLevels, setShowLevels] = useState(true);
const [traversalLoading, setTraversalLoading] = useState(false);
const [traversalError, setTraversalError] = useState('');

 const execute = async () => {
  setTraversalLoading(true);
  setTraversalError('');

  try {
    const response = await fetch(
      `${API_BASE_URL}/graph/traverse`,
      {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          algorithm,
          source,
        }),
      }
    );

    if (!response.ok) {
      throw new Error(`Traversal API error: ${response.status}`);
    }

    const data =
      (await response.json()) as TraversalApiResponse;

  if (
  data.success !== true ||
  !Array.isArray(data.traversal)
) {
  throw new Error('Invalid traversal API response');
}
  setResult({
  algorithm: data.algorithm,
  order: data.traversal,
  levels: data.levels ?? data.traversal.map((_, index) => index),
});
  } catch (error) {
    console.error('Traversal request failed:', error);

    setResult(null);
    setTraversalError(
      error instanceof Error
        ? error.message
        : 'Unable to run graph traversal.'
    );
  } finally {
    setTraversalLoading(false);
  }
};
  const names =
    result?.order.map(
      (id) => locations.find((l) => l.id === id)?.name ?? id
    ) ?? [];

  return (
    <>
      <SectionHeading
        eyebrow="NETWORK / CITY INFRASTRUCTURE"
        title="City Map & Network"
        description="Explore the 12-location city graph, road capacity and traversal behavior."
        action={<MockLabel />}
      />

      <div className="mini-stats">
        <span><b>12</b> locations</span>
        <span><b>15</b> bidirectional roads</span>
        <span><b>3</b> active emergency zones</span>
        <span><b>3</b> shelters</span>
      </div>

      <Panel
        title="Graph Exploration"
        action={
          <Pill tone={result ? 'success' : 'neutral'}>
            {result ? 'RESULT READY' : 'READY'}
          </Pill>
        }
      >
        <div
          className="form-stack"
          style={{
            padding: '16px',
            display: 'grid',
            gridTemplateColumns: '1fr 1fr auto',
            alignItems: 'end',
            gap: 12,
          }}
        >
          <label>
            Starting Location
            <select
              value={source}
              onChange={(e) => setSource(e.target.value)}
            >
              {locations.map((l) => (
                <option key={l.id} value={l.id}>
                  {l.name}
                </option>
              ))}
            </select>
          </label>

          <label>
            Traversal Algorithm
            <select
              value={algorithm}
              onChange={(e) =>
                setAlgorithm(e.target.value as 'BFS' | 'DFS')
              }
            >
              <option value="BFS">BFS — Breadth First Search</option>
              <option value="DFS">DFS — Depth First Search</option>
            </select>
          </label>

         <Button onClick={execute} disabled={traversalLoading}>
  <Play size={15} />
  {traversalLoading ? 'Running…' : `Run ${algorithm}`}
</Button>
    </div>

{traversalError && (
  <p
    role="alert"
    style={{ color: 'var(--destructive)', padding: '12px 16px' }}
  >
    {traversalError}
  </p>
)}

{result && (
  <div className="result-metrics">
            <div>
              <span>Algorithm</span>
              <strong>{result.algorithm}</strong>
            </div>
            <div>
              <span>Start</span>
              <strong>{result.order[0]}</strong>
            </div>
            <div>
              <span>Visited</span>
              <strong>{result.order.length}</strong>
            </div>
            <div>
              <span>Complexity</span>
              <strong>O(V + E)</strong>
            </div>
          </div>
        )}

        {result && (
          <div style={{ padding: '16px 20px 22px' }}>
            <div className="workflow-strip" style={{ margin: 0 }}>
              <span>
                <Binary size={15} /> {result.algorithm} TRAVERSAL
              </span>
              <span>{result.order.length} VISITED</span>
              <span>
                <Clock3 size={15} />
                {result.algorithm === 'BFS' ? 'LEVEL ORDER' : 'DEPTH ORDER'}
              </span>
            </div>

            <div className="route-steps">
              {names.map((name, i) => (
                <div
                  className="route-step"
                  key={`${name}-${i}`}
                >
                  <span className="route-step-num">
                    {String(i + 1).padStart(2, '0')}
                  </span>
                  <div>
                    <strong>{name}</strong>
                    <small>
                      {result.algorithm === 'BFS' && showLevels
                        ? `LEVEL ${result.levels[i]}`
                        : `STEP ${i + 1}`}
                    </small>
                  </div>
                </div>
              ))}
            </div>

            {result.algorithm === 'BFS' && (
              <Button
                variant="outline"
                size="sm"
                onClick={() => setShowLevels((v) => !v)}
              >
                {showLevels ? 'Hide levels' : 'Show BFS levels'}
              </Button>
            )}
          </div>
        )}
      </Panel>

      <Panel title="Interactive City Network" className="mt-panel">
        <CityGraph highlight={result?.order ?? []} />
      </Panel>

      <Panel title="Road Network" className="mt-panel">
        <DataTable
          headers={[
            'ROAD',
            'DISTANCE',
            'TRAVEL TIME',
            'CAPACITY',
            'STATUS',
          ]}
          rows={roads.map((r) => [
            `${r.from} ↔ ${r.to}`,
            `${r.distance} km`,
            `${r.time} min`,
            `${r.capacity} people / interval`,
            <Pill
              tone="success"
              key={`${r.from}-${r.to}`}
            >
              OPEN
            </Pill>,
          ])}
        />
      </Panel>
    </>
  );
}

export function EmergenciesPage() {
  type EmergencyApiItem = {
  id: string;
  type: string;
  location_id: string;
  severity: string;
  affected_people: number;
  required_resource: string;
  status: string;
};

type EmergencyApiResponse = {
  success: boolean;
  count: number;
  emergencies: EmergencyApiItem[];
};

function formatApiLabel(value: string): string {
  return value
    .toLowerCase()
    .replace(/\b\w/g, (letter) => letter.toUpperCase());
}

function mapApiEmergency(item: EmergencyApiItem): Emergency {
  const location = locations.find(
    (loc) => loc.id === item.location_id
  );

  const existing = initialEmergencies.find(
    (e) => e.id === item.id
  );

  return {
    id: item.id,
    type: formatApiLabel(item.type),
    location: location?.name ?? item.location_id,
    locationId: item.location_id,
    severity: formatApiLabel(item.severity),
    affected: item.affected_people,
    resource: formatApiLabel(item.required_resource),
    status: formatApiLabel(item.status),
    team: existing?.team ?? 'Unassigned',
    hospital: existing?.hospital ?? 'City Hospital',
    shelter: existing?.shelter ?? 'Central Shelter',
  };
}
  const [rows, setRows] = useState<Emergency[]>(initialEmergencies);
  const [selected, setSelected] = useState<Emergency | null>(null);
  

const [apiStatus, setApiStatus] = useState<
  'loading' | 'connected' | 'fallback'
>('loading');

useEffect(() => {
  let cancelled = false;

  async function loadEmergencies() {
    try {
      const response = await fetch(`${API_BASE_URL}/emergencies`);

      if (!response.ok) {
        throw new Error(`API error: ${response.status}`);
      }

      const data = (await response.json()) as EmergencyApiResponse;

      if (
        data.success !== true ||
        !Array.isArray(data.emergencies)
      ) {
        throw new Error('Invalid emergencies API response');
      }

      if (!cancelled) {
        setRows(data.emergencies.map(mapApiEmergency));
        setApiStatus('connected');
      }
    } catch (error) {
      console.error('Failed to load emergencies:', error);

      if (!cancelled) {
        setApiStatus('fallback');
      }
    }
  }

  void loadEmergencies();

  return () => {
    cancelled = true;
  };
}, []);
  const [reportOpen, setReportOpen] = useState(false);
  const [search, setSearch] = useState('');
  const [filter, setFilter] = useState('All');
  const [sortDesc, setSortDesc] = useState(true);
  const [newType, setNewType] = useState('Fire');
  const [newLocation, setNewLocation] = useState('L10');
  const [newSeverity, setNewSeverity] = useState('High');
  const [newAffected, setNewAffected] = useState('100');

  const filtered = useMemo(
    () =>
      rows
        .filter(
          (e) =>
            (filter === 'All' || e.severity === filter) &&
            `${e.id} ${e.type} ${e.location} ${e.resource}`
              .toLowerCase()
              .includes(search.toLowerCase())
        )
        .sort((a, b) =>
          sortDesc
            ? b.affected - a.affected
            : a.affected - b.affected
        ),
    [rows, filter, search, sortDesc]
  );

  const addEmergency = (ev: FormEvent) => {
    ev.preventDefault();
    const loc = locations.find((l) => l.id === newLocation)!;

    const next: Emergency = {
      id: `E${rows.length + 1}`,
      type: newType,
      location: loc.name,
      locationId: loc.id,
      severity: newSeverity,
      affected: Math.max(1, Number(newAffected) || 1),
      resource:
        newType === 'Fire'
          ? 'Water'
          : newType === 'Flood'
            ? 'Food'
            : 'Medicine',
      status: 'Active',
      team: 'Delta Team',
      hospital: 'City Hospital',
      shelter: 'Central Shelter',
    };

    setRows((prev) => [...prev, next]);
    setReportOpen(false);
  };

  return (
    <>
      <SectionHeading
        eyebrow="INCIDENTS / ACTIVE RESPONSE"
        title="Emergency Management"
        description="Search, filter, prioritize and review active incidents."
        action={
          <Button onClick={() => setReportOpen(true)}>
            <Plus size={16} /> Report Emergency
          </Button>
        }
      />

      <div className="stats-grid">
        <Stat
          icon={Siren}
          label="Total Emergencies"
          value={String(rows.length)}
          note="Current registry"
          tone="info"
        />
        <Stat
          icon={AlertTriangle}
          label="Critical"
          value={String(rows.filter((e) => e.severity === 'Critical').length)}
          note="Immediate response"
          tone="critical"
        />
        <Stat
          icon={Flame}
          label="High Severity"
          value={String(rows.filter((e) => e.severity === 'High').length)}
          note="Priority monitoring"
          tone="warning"
        />
        <Stat
          icon={CheckCircle2}
          label="Resolved"
          value={String(rows.filter((e) => e.status === 'Resolved').length)}
          note="Closed incidents"
          tone="success"
        />
      </div>

     <Panel
  title="Incident Registry"
  action={
    <Pill
      tone={
        apiStatus === 'connected'
          ? 'success'
          : apiStatus === 'loading'
            ? 'neutral'
            : 'warning'
      }
    >
      {apiStatus === 'connected'
        ? 'LIVE DATA'
        : apiStatus === 'loading'
          ? 'LOADING'
          : 'LOCAL FALLBACK'}
    </Pill>
  }
>
        <div
          style={{
            padding: '15px 20px 0',
            display: 'flex',
            gap: 10,
            flexWrap: 'wrap',
            alignItems: 'center',
          }}
        >
          <div className="search-box" style={{ maxWidth: 330 }}>
            <Search size={17} />
            <Input
              placeholder="Search incident, type or location..."
              value={search}
              onChange={(e) => setSearch(e.target.value)}
            />
          </div>

          <Select value={filter} onValueChange={setFilter}>
            <SelectTrigger className="unit-select">
              <SelectValue placeholder="Severity" />
            </SelectTrigger>
            <SelectContent>
              {['All', 'Critical', 'High', 'Medium', 'Low'].map((v) => (
                <SelectItem value={v} key={v}>
                  {v} severity
                </SelectItem>
              ))}
            </SelectContent>
          </Select>

          <Button
            variant="outline"
            size="sm"
            onClick={() => setSortDesc((v) => !v)}
          >
            <SlidersHorizontal size={15} /> Affected {sortDesc ? '↓' : '↑'}
          </Button>

          <span className="result-count" style={{ marginLeft: 'auto' }}>
            {filtered.length} matching incidents
          </span>
        </div>

        <EmergencyTable
          rows={filtered}
          withActions
          onSelect={setSelected}
        />
      </Panel>

      <Dialog
        open={Boolean(selected)}
        onOpenChange={(v) => !v && setSelected(null)}
      >
        <DialogContent className="dialog-wide">
          <DialogHeader>
            <DialogTitle>
              Incident {selected?.id} · {selected?.type}
            </DialogTitle>
            <DialogDescription>
              Front-end response planning preview
            </DialogDescription>
          </DialogHeader>

          {selected && (
            <div className="detail-dialog">
              <div className="detail-dialog-top">
                <div>
                  <span>LOCATION</span>
                  <strong>{selected.location}</strong>
                </div>
                <div>
                  <span>SEVERITY</span>
                  <Pill tone={severityTone(selected.severity)}>
                    {selected.severity.toUpperCase()}
                  </Pill>
                </div>
                <div>
                  <span>STATUS</span>
                  <Pill tone="critical">
                    {selected.status.toUpperCase()}
                  </Pill>
                </div>
              </div>

              <div className="detail-pairs">
                {[
                  ['Affected people', selected.affected.toLocaleString()],
                  ['Required resource', selected.resource],
                  ['Nearest rescue team', selected.team],
                  ['Nearest hospital', selected.hospital],
                  ['Recommended shelter', selected.shelter],
                  [
                    'Recommended route',
                    routeForIncident(
                      selected.locationId,
                      shelters.find((s) => s.name === selected.shelter)
                        ?.locationId ?? 'L7'
                    )?.names.join(' → ') ?? 'Unavailable',
                  ],
                ].map(([k, v]) => (
                  <div key={k}>
                    <span>{k}</span>
                    <strong>{v}</strong>
                  </div>
                ))}
              </div>
            </div>
          )}
        </DialogContent>
      </Dialog>

      <Dialog open={reportOpen} onOpenChange={setReportOpen}>
        <DialogContent>
          <DialogHeader>
            <DialogTitle>Report Emergency</DialogTitle>
            <DialogDescription>
              Local front-end entry; backend persistence comes later.
            </DialogDescription>
          </DialogHeader>

          <form className="form-stack" onSubmit={addEmergency}>
            <label>
              Disaster Type
              <select
                value={newType}
                onChange={(e) => setNewType(e.target.value)}
              >
                <option>Fire</option>
                <option>Flood</option>
                <option>Accident</option>
              </select>
            </label>

            <label>
              Location
              <select
                value={newLocation}
                onChange={(e) => setNewLocation(e.target.value)}
              >
                {locations.map((l) => (
                  <option key={l.id} value={l.id}>
                    {l.name}
                  </option>
                ))}
              </select>
            </label>

            <label>
              Severity
              <select
                value={newSeverity}
                onChange={(e) => setNewSeverity(e.target.value)}
              >
                <option>Low</option>
                <option>Medium</option>
                <option>High</option>
                <option>Critical</option>
              </select>
            </label>

            <label>
              Affected People
              <Input
                type="number"
                min="1"
                value={newAffected}
                onChange={(e) => setNewAffected(e.target.value)}
              />
            </label>

            <Button type="submit">Add to local registry</Button>
          </form>
        </DialogContent>
      </Dialog>
    </>
  );
}
type EvacuationApiResponse = {
  success: boolean;
  algorithm: string;
  source: string;
  source_name: string;
  destination: string;
  destination_name: string;
  distance: number;
  estimated_time: number;
  path: string[];
  path_names: string[];
  time_complexity: string;
};
export function EvacuationPage() {
  const [start, setStart] = useState('L10');
  const [end, setEnd] = useState('L7');
  const [criteria, setCriteria] =
    useState<'distance' | 'time' | 'capacity'>('distance');

 const [result, setResult] = useState<{
  ids: string[];
  names: string[];
  distance: number;
  time: number;
  capacity?: number;
} | null>(() => previewRoute('L10', 'L7', 'distance'));

const [loading, setLoading] = useState(false);
const [routeError, setRouteError] = useState('');
  const calculate = async () => {
  setLoading(true);
  setRouteError('');

  // The backend Dijkstra endpoint optimizes distance.
  // Keep the existing local previews for the other criteria.
  if (criteria !== 'distance') {
    setResult(previewRoute(start, end, criteria));
    setLoading(false);
    return;
  }

  try {
    const response = await fetch(
      `${API_BASE_URL}/evacuation/route`,
      {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          source: start,
          destination: end,
        }),
      }
    );

    if (!response.ok) {
      throw new Error(`Dijkstra API error: ${response.status}`);
    }

    const data =
      (await response.json()) as EvacuationApiResponse;

    if (
      data.success !== true ||
      !Array.isArray(data.path) ||
      !Array.isArray(data.path_names) ||
      typeof data.distance !== 'number' ||
      typeof data.estimated_time !== 'number'
    ) {
      throw new Error('Invalid Dijkstra API response');
    }

    setResult({
      ids: data.path,
      names: data.path_names,
      distance: data.distance,
      time: data.estimated_time,
    });
  } catch (error) {
    console.error('Dijkstra request failed:', error);

    setResult(null);
    setRouteError(
      error instanceof Error
        ? error.message
        : 'Unable to calculate the evacuation route.'
    );
  } finally {
    setLoading(false);
  }
};

  return (
    <>
      <SectionHeading
        eyebrow="OPTIMIZATION / SHORTEST PATH"
        title="Evacuation Route Optimization"
        description="Compare routes across the city graph to support safe evacuation decisions."
        action={<MockLabel />}
      />

      <div className="workflow-strip">
        <span><MapPinned size={16} /> CITY GRAPH</span>
        <ArrowRight size={15} />
        <span><Binary size={16} /> DIJKSTRA</span>
        <ArrowRight size={15} />
        <span><Gauge size={16} /> CALCULATION</span>
        <ArrowRight size={15} />
        <span><ShieldCheck size={16} /> DECISION</span>
      </div>

      <div className="evacuation-layout">
        <Panel title="Route Parameters">
          <div className="form-stack">
            <label>
              Emergency Location
              <Select value={start} onValueChange={setStart}>
                <SelectTrigger>
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                  {locations.map((l) => (
                    <SelectItem key={l.id} value={l.id}>
                      {l.name}
                    </SelectItem>
                  ))}
                </SelectContent>
              </Select>
            </label>

            <label>
              Destination Shelter
              <Select value={end} onValueChange={setEnd}>
                <SelectTrigger>
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                  {shelters.map((s) => (
                    <SelectItem key={s.id} value={s.locationId}>
                      {s.name}
                    </SelectItem>
                  ))}
                </SelectContent>
              </Select>
            </label>

            <label>
              Optimization Criteria
              <Select
                value={criteria}
                onValueChange={(v) => setCriteria(v as typeof criteria)}
              >
                <SelectTrigger>
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                  <SelectItem value="distance">Shortest Distance</SelectItem>
                  <SelectItem value="time">Minimum Time</SelectItem>
                  <SelectItem value="capacity">Maximum Capacity</SelectItem>
                </SelectContent>
              </Select>
            </label>

            <Button
              className="w-full mt-2"
              onClick={calculate}
              disabled={loading || start === end}
            >
              <RouteIcon />
              {loading ? 'Calculating…' : 'Calculate Optimal Route'}
            </Button>
          </div>

          <div className="aside-hint">
            <Binary size={18} />
            <p>
             <strong>Dijkstra optimization</strong>
<br />
Shortest-distance calculations use the Flask backend API.
Minimum-time and maximum-capacity options still use the local preview.
            </p>
          </div>
        </Panel>

        <Panel title="Optimization Result">
          {routeError && (
            
  <p
    role="alert"
    style={{
      color: 'var(--destructive)',
      padding: '12px 16px',
    }}
  >
    {routeError}
  </p>
)}
          {loading ? (
            <div className="result-empty">
              <Activity size={30} />
              <strong>Calculating route…</strong>
              <p>Running the front-end route preview.</p>
            </div>
          ) : !result ? (
            <div className="result-empty">
              <RouteIcon size={30} />
              <strong>No route found</strong>
              <p>Choose a source and destination.</p>
            </div>
          ) : (
            <>
              <div className="result-banner">
                <div>
                  <span>RECOMMENDED ROUTE · DIJKSTRA PREVIEW</span>
                  <strong>
                    {locations.find((l) => l.id === start)?.name} →{' '}
                    {shelters.find((s) => s.locationId === end)?.name}
                  </strong>
                </div>
                <Pill tone="success">ROUTE FOUND</Pill>
              </div>

              <div className="route-steps">
                {result.names.map((name, i) => (
                  <div className="route-step" key={`${name}-${i}`}>
                    <span className="route-step-num">
                      {String(i + 1).padStart(2, '0')}
                    </span>
                    <div>
                      <strong>{name}</strong>
                      <small>
                        {i === 0
                          ? 'ORIGIN'
                          : i === result.names.length - 1
                            ? 'DESTINATION'
                            : 'VIA'}
                      </small>
                    </div>
                  </div>
                ))}
              </div>

              <div className="result-metrics">
                <div>
                  <span>Distance</span>
                  <strong>{result.distance} km</strong>
                </div>
                <div>
                  <span>Estimated time</span>
                  <strong>{result.time} min</strong>
                </div>
                <div>
                  <span>Capacity</span>
                <strong>
  {typeof result.capacity === 'number'
    ? `${result.capacity} / interval`
    : 'Not provided'}
</strong>
                </div>
                <div>
                  <span>Nodes</span>
                  <strong>{result.ids.length}</strong>
                </div>
              </div>
            </>
          )}
        </Panel>
      </div>

      <Panel title="Route Visualization" className="mt-panel">
        <CityGraph compact highlight={result?.ids ?? []} />
      </Panel>
    </>
  );
}
type PageApiStatus = 'loading' | 'connected' | 'fallback';

type RescueTeamApiItem = {
  id: string;
  name: string;
  type: string;
  location_id: string;
  capacity: number;
  status: string;
};

type RescueTeamsApiResponse = {
  success: boolean;
  count: number;
  rescue_teams: RescueTeamApiItem[];
};

type RescueMatchApiResponse = {
  success: boolean;
  assignments: {
    team_id: string;
    emergency_id: string;
  }[];
};

type ShelterApiItem = {
  id: string;
  name: string;
  location_id: string;
  total_capacity: number;
  occupied: number;
  available: number;
  status: string;
};

type SheltersApiResponse = {
  success: boolean;
  count: number;
  shelters: ShelterApiItem[];
};
export function TeamsPage() {
  const [teams, setTeams] = useState(initialTeams);
const [assignments, setAssignments] = useState<Record<string, string>>({});
const [showOnlyAvailable, setShowOnlyAvailable] = useState(false);
const [teamApiStatus, setTeamApiStatus] =
  useState<PageApiStatus>('loading');

useEffect(() => {
  let cancelled = false;

  async function loadRescueData() {
    try {
      const [teamsResponse, matchResponse] = await Promise.all([
        fetch(`${API_BASE_URL}/rescue-teams`),
        fetch(`${API_BASE_URL}/rescue-teams/match`),
      ]);

      if (!teamsResponse.ok || !matchResponse.ok) {
        throw new Error('Failed to load rescue team data.');
      }

      const teamData =
        (await teamsResponse.json()) as RescueTeamsApiResponse;

      const matchData =
        (await matchResponse.json()) as RescueMatchApiResponse;

      if (
        teamData.success !== true ||
        !Array.isArray(teamData.rescue_teams) ||
        matchData.success !== true ||
        !Array.isArray(matchData.assignments)
      ) {
        throw new Error('Invalid rescue team API response.');
      }

      const mappedTeams = teamData.rescue_teams.map((team) => ({
        id: team.id,
        name: team.name,
        type: team.type.toLowerCase().replace(/\b\w/g, (c) => c.toUpperCase()),
        location:
          locations.find((location) => location.id === team.location_id)
            ?.name ?? team.location_id,
        capacity: team.capacity,
        status:
          team.status.toLowerCase() === 'available'
            ? 'Available'
            : 'Assigned',
      }));

      const mappedAssignments = Object.fromEntries(
        matchData.assignments.map((assignment) => [
          assignment.team_id,
          assignment.emergency_id,
        ])
      );

      if (!cancelled) {
        setTeams(mappedTeams as typeof initialTeams);
        setAssignments(mappedAssignments);
        setTeamApiStatus('connected');
      }
    } catch (error) {
      console.error('Failed to load rescue teams:', error);

      if (!cancelled) {
        setTeamApiStatus('fallback');
      }
    }
  }

  void loadRescueData();

  return () => {
    cancelled = true;
  };
}, []);
  const emergencies = initialEmergencies;

  const assign = (teamId: string, emergencyId: string) =>
    setAssignments((prev) => ({ ...prev, [teamId]: emergencyId }));

  const available = teams.filter(
    (t) => !showOnlyAvailable || t.status === 'Available'
  );

  const assignmentByTeam = new Map(Object.entries(assignments));

  return (
    <>
      <SectionHeading
        eyebrow="RESPONSE / PERSONNEL"
        title="Rescue Teams"
        description="Manage response capacity and preview team-to-emergency assignment."
        action={
  <Pill
    tone={
      teamApiStatus === 'connected'
        ? 'success'
        : teamApiStatus === 'loading'
          ? 'neutral'
          : 'warning'
    }
  >
    {teamApiStatus === 'connected'
      ? 'LIVE DATA'
      : teamApiStatus === 'loading'
        ? 'LOADING'
        : 'LOCAL FALLBACK'}
  </Pill>
}
      />

      <div className="stats-grid">
        <Stat
          icon={Users}
          label="Total Teams"
          value={String(teams.length)}
          note="Registered response units"
          tone="info"
        />
        <Stat
          icon={ShieldCheck}
          label="Available"
          value={String(
            teams.filter((t) => t.status === 'Available').length
          )}
          note="Ready to dispatch"
          tone="success"
        />
        <Stat
          icon={Activity}
          label="Assigned"
          value={String(Object.keys(assignments).length)}
          note="Current front-end plan"
          tone="warning"
        />
        <Stat
          icon={Gauge}
          label="Total Capacity"
          value={String(teams.reduce((a, t) => a + t.capacity, 0))}
          note="People per response"
          tone="info"
        />
      </div>

      <div
        style={{
          display: 'flex',
          justifyContent: 'flex-end',
          margin: '10px 0 15px',
        }}
      >
        <Button
          variant={showOnlyAvailable ? 'default' : 'outline'}
          onClick={() => setShowOnlyAvailable((v) => !v)}
        >
          <ShieldCheck size={15} />
          {showOnlyAvailable ? 'Showing available only' : 'Filter available'}
        </Button>
      </div>

      <div className="team-grid">
        {available.map((t) => {
          const assigned = assignmentByTeam.get(t.id);

          return (
            <Panel key={t.id}>
              <div className="team-card" style={{ border: 0, padding: 0 }}>
                <div className="team-card-top">
                  <div>
                    <span className="eyebrow">{t.id}</span>
                    <h2>{t.name}</h2>
                    <p>{t.type} response · {t.location}</p>
                  </div>
                  <Pill tone={t.status === 'Available' ? 'success' : 'warning'}>
                    {t.status.toUpperCase()}
                  </Pill>
                </div>

                <div className="team-card-stats">
                  <span><b>{t.capacity}</b> capacity</span>
                  <span><b>{assigned ?? '—'}</b> assignment</span>
                </div>

                <label className="form-stack">
                  Assign emergency
                  <select
                    value={assigned ?? ''}
                    onChange={(e) => assign(t.id, e.target.value)}
                  >
                    <option value="">Unassigned</option>
                    {emergencies.map((e) => (
                      <option value={e.id} key={e.id}>
                        {e.id} · {e.type}
                      </option>
                    ))}
                  </select>
                </label>

                <div className="team-card-foot">
                  <span>Compatibility preview</span>
                  <strong>
                    {t.type === 'General'
                      ? 'General support'
                      : `Best for ${t.type}`}
                  </strong>
                </div>
              </div>
            </Panel>
          );
        })}
      </div>

      <Panel title="Assignment Plan" className="mt-panel">
        <DataTable
          headers={[
            'TEAM',
            'SPECIALTY',
            'ASSIGNMENT',
            'CAPACITY',
            'STATUS',
          ]}
          rows={teams.map((t) => [
            t.name,
            t.type,
            assignmentByTeam.get(t.id) ?? 'Unassigned',
            t.capacity,
            <Pill tone="success" key={t.id}>
              READY
            </Pill>,
          ])}
        />
      </Panel>
    </>
  );
}
export function SheltersPage() {
  const [selected, setSelected] = useState<string | null>(null);
  
const [shelterRows, setShelterRows] = useState(shelters);

const [shelterApiStatus, setShelterApiStatus] =
  useState<PageApiStatus>('loading');

useEffect(() => {
  let cancelled = false;

  async function loadShelters() {
    try {
      const response = await fetch(`${API_BASE_URL}/shelters`);

      if (!response.ok) {
        throw new Error(`Shelters API error: ${response.status}`);
      }

      const data =
        (await response.json()) as SheltersApiResponse;

      if (
        data.success !== true ||
        !Array.isArray(data.shelters)
      ) {
        throw new Error('Invalid shelters API response.');
      }

      const mappedShelters = data.shelters.map((shelter) => ({
        id: shelter.id,
        locationId: shelter.location_id,
        name: shelter.name,
        capacity: shelter.total_capacity,
        occupied: shelter.occupied,
      })) as typeof shelters;

      if (!cancelled) {
        setShelterRows(mappedShelters);
        setShelterApiStatus('connected');
      }
    } catch (error) {
      console.error('Failed to load shelters:', error);

      if (!cancelled) {
        setShelterApiStatus('fallback');
      }
    }
  }

  void loadShelters();

  return () => {
    cancelled = true;
  };
}, []);
 const total = shelterRows.reduce((a, s) => a + s.capacity, 0);
const occupied = shelterRows.reduce((a, s) => a + s.occupied, 0);
const available = total - occupied;

const selectedShelter = shelterRows.find((s) => s.id === selected);
  return (
    <>
      <SectionHeading
        eyebrow="SAFE HAVENS / CAPACITY"
        title="Shelter Management"
        description="Monitor occupancy, available space and shelter readiness."
      action={
  <Pill
    tone={
      shelterApiStatus === 'connected'
        ? 'success'
        : shelterApiStatus === 'loading'
          ? 'neutral'
          : 'warning'
    }
  >
    {shelterApiStatus === 'connected'
      ? 'LIVE DATA'
      : shelterApiStatus === 'loading'
        ? 'LOADING'
        : 'LOCAL FALLBACK'}
  </Pill>
}
      />

      <div className="stats-grid">
        <Stat
          icon={House}
          label="Total Capacity"
          value={total.toLocaleString()}
          note="Across 3 shelters"
          tone="info"
        />
        <Stat
          icon={Users}
          label="Occupied"
          value={occupied.toLocaleString()}
          note={`${((occupied / total) * 100).toFixed(1)}% occupied`}
          tone="warning"
        />
        <Stat
          icon={CheckCircle2}
          label="Available"
          value={available.toLocaleString()}
          note="Ready intake capacity"
          tone="success"
        />
        <Stat
          icon={ShieldCheck}
          label="Open Shelters"
          value={String(shelters.length)}
          note="All currently operational"
          tone="info"
        />
      </div>

      <Panel title="Shelter Capacity Overview">
       <ShelterCards
  detailed
  onSelect={setSelected}
  rows={shelterRows}
/>
      </Panel>

      <Panel title="Capacity Distribution" className="mt-panel">
        <div style={{ height: 270 }}>
          <ResponsiveContainer width="100%" height="100%">
            <PieChart>
              <Pie
                data={shelters.map((s) => ({
                  name: s.name,
                  value: s.occupied,
                }))}
                dataKey="value"
                nameKey="name"
                innerRadius={60}
                outerRadius={95}
                label
              >
                {shelters.map((s, i) => (
                  <Cell
                    key={s.id}
                    fill={`hsl(${210 + i * 35} 65% ${55 + i * 5}%)`}
                  />
                ))}
              </Pie>
              <Tooltip />
              <Legend />
            </PieChart>
          </ResponsiveContainer>
        </div>
      </Panel>

      <Panel title="Shelter Details" className="mt-panel">
        <DataTable
          headers={[
            'SHELTER',
            'LOCATION',
            'CAPACITY',
            'OCCUPIED',
            'AVAILABLE',
            'STATUS',
          ]}
          rows={shelters.map((s) => [
            s.name,
            s.locationId,
            s.capacity,
            s.occupied,
            s.capacity - s.occupied,
            <Pill tone="success" key={s.id}>OPEN</Pill>,
          ])}
        />
      </Panel>

      <Dialog
        open={Boolean(selected)}
        onOpenChange={(v) => !v && setSelected(null)}
      >
        <DialogContent>
          <DialogHeader>
            <DialogTitle>{selectedShelter?.name}</DialogTitle>
            <DialogDescription>
              Front-end shelter monitoring
            </DialogDescription>
          </DialogHeader>

          {selectedShelter && (
            <div className="detail-pairs">
              <div>
                <span>Total capacity</span>
                <strong>{selectedShelter.capacity}</strong>
              </div>
              <div>
                <span>Occupied</span>
                <strong>{selectedShelter.occupied}</strong>
              </div>
              <div>
                <span>Available</span>
                <strong>
                  {selectedShelter.capacity - selectedShelter.occupied}
                </strong>
              </div>
              <div>
                <span>Occupancy</span>
                <strong>
                  {(
                    (selectedShelter.occupied / selectedShelter.capacity) *
                    100
                  ).toFixed(1)}
                  %
                </strong>
              </div>
            </div>
          )}
        </DialogContent>
      </Dialog>
    </>
  );
}
type ResourceInventoryApiItem = {
  id: string;
  location_id: string;
  priority: number;
  quantity: number;
  type: string;
  unit: string;
};

type ResourceInventoryApiResponse = {
  success: boolean;
  count: number;
  resources: ResourceInventoryApiItem[];
};

type KnapsackApiResponse = {
  success: boolean;
  algorithm: string;
  allocated_quantity: number;
  capacity: number;
  remaining_capacity: number;
  resource_type: string;
  selected_resources: {
    resource_id: string;
    location_id: string;
    type: string;
    unit: string;
    allocated_quantity: number;
    allocated_value: number;
    available_quantity: number;
    priority: number;
  }[];
  total_priority_value: number;
};

type CoinChangeApiResponse = {
  success: boolean;
  amount: number;
  denominations: number[];
  minimum_coins: number;
  reachable: boolean;
  solution: {
    count: number;
    denomination: number;
  }[];
};

export function ResourcesPage() {
  const [selectedPriority, setSelectedPriority] = useState(1);
const [capacity, setCapacity] = useState('1200');

const [resourceRows, setResourceRows] = useState(resources);
const [resourceApiStatus, setResourceApiStatus] =
  useState<'loading' | 'connected' | 'fallback'>('loading');

const [optimized, setOptimized] =
  useState<KnapsackApiResponse | null>(null);

const [allocationLoading, setAllocationLoading] = useState(false);
const [allocationError, setAllocationError] = useState('');

const [coinAmount, setCoinAmount] = useState('63');
const [coinResult, setCoinResult] =
  useState<CoinChangeApiResponse | null>(null);
const [coinLoading, setCoinLoading] = useState(false);
const [coinError, setCoinError] = useState('');

useEffect(() => {
  let cancelled = false;

  async function loadResources() {
    try {
      const response = await fetch(`${API_BASE_URL}/resources`);

      if (!response.ok) {
        throw new Error(`Resources API error: ${response.status}`);
      }

      const data =
        (await response.json()) as ResourceInventoryApiResponse;

      if (
        data.success !== true ||
        !Array.isArray(data.resources)
      ) {
        throw new Error('Invalid resources API response');
      }

      const mappedResources = data.resources.map((item) => ({
        id: item.id,
        name: item.type.toLowerCase().replace(/\b\w/g, (c) => c.toUpperCase()),
        location:
          locations.find((location) => location.id === item.location_id)
            ?.name ?? item.location_id,
        quantity: item.quantity,
        unit: item.unit,
        priority: item.priority,
      })) as typeof resources;

      if (!cancelled) {
        setResourceRows(mappedResources);
        setResourceApiStatus('connected');
      }
    } catch (error) {
      console.error('Failed to load resources:', error);

      if (!cancelled) {
        setResourceApiStatus('fallback');
      }
    }
  }

  void loadResources();

  return () => {
    cancelled = true;
  };
}, []);

const filtered = resourceRows.filter(
  (resource) => resource.priority >= selectedPriority
);

  const runAllocation = async () => {
  const amount = Number(capacity);

  if (!Number.isFinite(amount) || amount <= 0) {
    setAllocationError('Enter a capacity greater than zero.');
    return;
  }

  setAllocationLoading(true);
  setAllocationError('');

  try {
    const response = await fetch(
      `${API_BASE_URL}/resources/allocate?resource_type=WATER&capacity=${encodeURIComponent(String(amount))}`
    );

    if (!response.ok) {
      throw new Error(`Allocation API error: ${response.status}`);
    }

    const data =
      (await response.json()) as KnapsackApiResponse;

    if (
      data.success !== true ||
      !Array.isArray(data.selected_resources)
    ) {
      throw new Error('Invalid allocation API response');
    }

    setOptimized(data);
  } catch (error) {
    console.error('Resource allocation failed:', error);

    setOptimized(null);
    setAllocationError(
      error instanceof Error
        ? error.message
        : 'Unable to allocate resources.'
    );
  } finally {
    setAllocationLoading(false);
  }
};
useEffect(() => {
  let cancelled = false;
  const amount = Number(coinAmount);

  if (
    coinAmount.trim() === '' ||
    !Number.isInteger(amount) ||
    amount < 0
  ) {
    setCoinResult(null);
    setCoinError('Enter a non-negative whole number.');
    setCoinLoading(false);
    return;
  }

  async function loadCoinChange() {
    setCoinLoading(true);
    setCoinError('');

    try {
      const response = await fetch(
        `${API_BASE_URL}/algorithms/coin-change?amount=${encodeURIComponent(String(amount))}&denominations=1,5,10,25`
      );

      if (!response.ok) {
        throw new Error(`Coin Change API error: ${response.status}`);
      }

      const data =
        (await response.json()) as CoinChangeApiResponse;

      if (
        data.success !== true ||
        !Array.isArray(data.solution) ||
        typeof data.reachable !== 'boolean'
      ) {
        throw new Error('Invalid Coin Change API response');
      }

      if (!cancelled) {
        setCoinResult(data);
      }
    } catch (error) {
      console.error('Coin Change request failed:', error);

      if (!cancelled) {
        setCoinResult(null);
        setCoinError(
          error instanceof Error
            ? error.message
            : 'Unable to calculate Coin Change.'
        );
      }
    } finally {
      if (!cancelled) {
        setCoinLoading(false);
      }
    }
  }

  void loadCoinChange();

  return () => {
    cancelled = true;
  };
}, [coinAmount]);
  return (
    <>
     <SectionHeading
  eyebrow="LOGISTICS / SUPPLY CHAIN"
  title="Resource Management"
  description="Monitor inventory priority and run front-end allocation previews."
  action={<MockLabel />}
/>

      <div className="resource-cards">
        {resources.map((r) => (
          <div className="resource-card" key={r.id}>
            <div className="resource-card-top">
              <span className="resource-icon">
                <Droplets size={17} />
              </span>
              <Pill tone={r.priority >= 9 ? 'warning' : 'info'}>
                {r.priority} / 10
              </Pill>
            </div>

            <h3>{r.name}</h3>
            <p>{r.location}</p>
            <strong>{r.quantity.toLocaleString()}</strong>
            <span>{r.unit}</span>

            <CapacityBar
              value={Math.min(100, r.priority * 10)}
              tone={r.priority >= 9 ? 'warning' : 'success'}
            />
          </div>
        ))}
      </div>

    <div className="evacuation-layout" style={{ marginTop: 20 }}>
  <Panel title="Fractional Knapsack Preview">
    <div className="form-stack">
      <label>
        Water Allocation Capacity (liters)
        <Input
          type="number"
          min="1"
          value={capacity}
          onChange={(e) => setCapacity(e.target.value)}
        />
      </label>

      <Button
        onClick={runAllocation}
        disabled={allocationLoading}
      >
        <Gauge size={15} />
        {allocationLoading ? 'Optimizing…' : 'Optimize Loading'}
      </Button>

      {allocationError && (
        <p role="alert" style={{ color: 'var(--destructive)' }}>
          {allocationError}
        </p>
      )}
    </div>

    {optimized && (
      <div className="simulation-summary">
        {optimized.selected_resources.map((item) => (
          <div key={item.resource_id}>
            <span>
              {item.resource_id} ·{' '}
              {locations.find((l) => l.id === item.location_id)?.name ??
                item.location_id}
            </span>

            <strong>
              {item.allocated_quantity.toLocaleString()} {item.unit}
              {' · Value '}
              {item.allocated_value.toLocaleString()}
            </strong>
          </div>
        ))}

        <div>
          <span>Total allocated</span>
          <strong>
          {optimized.allocated_quantity.toLocaleString()}{' '}
{optimized.selected_resources[0]?.unit ?? ''}
          </strong>
        </div>

        <div>
          <span>Total priority value</span>
          <strong>
            {optimized.total_priority_value.toLocaleString()}
          </strong>
        </div>

        <div>
          <span>Remaining capacity</span>
          <strong>
            {optimized.remaining_capacity.toLocaleString()}{' '}
{optimized.selected_resources[0]?.unit ?? ''}
          </strong>
        </div>
      </div>
    )}
  </Panel>

  <Panel title="Coin Change Preview">
    <div className="form-stack">
      <label>
        Required Resource Packages
        <Input
          type="number"
          min="0"
          value={coinAmount}
          onChange={(e) => setCoinAmount(e.target.value)}
        />
      </label>
    </div>

    <div className="result-banner">
      <div>
        <span>MINIMUM PACKAGES</span>
        <strong>
          {coinLoading
            ? 'Calculating…'
            : coinResult
              ? coinResult.reachable
                ? coinResult.minimum_coins
                : 'Not possible'
              : '—'}
        </strong>
      </div>

      <Pill
        tone={
          coinResult
            ? 'success'
            : coinLoading
              ? 'neutral'
              : 'warning'
        }
      >
        {coinResult
          ? 'BACKEND RESULT'
          : coinLoading
            ? 'LOADING'
            : 'NO RESULT'}
      </Pill>
    </div>

    {coinError && (
      <p role="alert" style={{ color: 'var(--destructive)', padding: 16 }}>
        {coinError}
      </p>
    )}

    <div className="alternative">
      <strong>Denominations</strong>
      <p>
        {coinResult
          ? coinResult.reachable
            ? coinResult.solution
                .map(
                  (item) => `${item.count} × ${item.denomination}`
                )
                .join(' · ')
            : 'Not possible with these denominations'
          : coinLoading
            ? 'Calculating…'
            : 'No result available'}
      </p>
    </div>
  </Panel>
</div>

      <Panel title="Inventory Registry" className="mt-panel">
        <div
          style={{
            padding: 16,
            display: 'flex',
            gap: 10,
            alignItems: 'center',
          }}
        >
          <span>Priority at least</span>
          <input
            type="range"
            min="1"
            max="10"
            value={selectedPriority}
            onChange={(e) => setSelectedPriority(Number(e.target.value))}
          />
          <strong>{selectedPriority}</strong>
          <span style={{ marginLeft: 'auto' }}>
            {filtered.length} matching
          </span>
        </div>

        <DataTable
          headers={[
            'ID',
            'RESOURCE',
            'LOCATION',
            'QUANTITY',
            'PRIORITY',
            'STATUS',
          ]}
          rows={filtered.map((r) => [
            r.id,
            <strong key={r.id}>{r.name.toUpperCase()}</strong>,
            r.location,
            `${r.quantity.toLocaleString()} ${r.unit}`,
            <Pill
              tone={r.priority >= 9 ? 'warning' : 'info'}
              key={`${r.id}-priority`}
            >
              {r.priority} / 10
            </Pill>,
            <Pill tone="success" key={`${r.id}-status`}>
              AVAILABLE
            </Pill>,
          ])}
        />
      </Panel>
    </>
  );
}
const algorithmDemoOutputs: Record<string, string> = {
  Searching: 'Search L10 → Industrial Area. Best case O(1), average/worst O(n).',
  Sorting: 'Affected people descending: E2 (250), E1 (150), E3 (50).',
  BFS: 'L10 traversed level-by-level across all 12 locations.',
  DFS: 'L10 traversed depth-first across all reachable locations.',
  'Topological Sort': 'Detect → Assess → Dispatch → Allocate → Open Route → Evacuate → Shelter.',
  Recursion: 'Disaster-zone hierarchy expanded recursively from the root.',
  'Merge Sort': 'Emergency counts sorted to 250, 150, 50.',
  'Quick Sort': 'Emergency counts sorted to 250, 150, 50.',
  'Closest Pair': 'Closest response points: West Junction and South Junction, distance ≈ 14.14.',
  'Majority Element': 'FIRE is the majority emergency type in the demonstration dataset.',
  'Order Statistics': '3rd most affected emergency: E7, FIRE, 220 people.',
  Euclid: 'GCD(1000, 500) = 500.',
  Karatsuba: '1234 × 5678 = 7,006,652.',
  'Fast Modular Exponentiation': '7^20 mod 1000 = 1.',
  Josephus: '7 teams, step 3 → survivor Team 4.',
  'Tower of Hanoi': '3 disks → 7 moves.',
  'Inversion Count': 'Sample emergency sequence contains 4 inversions.',
  'Randomized Algorithms': 'Randomized quicksort sorts the emergency priority sequence.',
  Fibonacci: 'F(10) = 55 using a DP table.',
  'Binomial Coefficient': 'C(6, 2) = 15.',
  'Matrix Chain Multiplication': 'Minimum scalar multiplications = 29,000.',
  'Coin Change': 'Amount 63 → minimum 6 packages using 1, 5, 10, 25.',
  TSP: 'Best rescue tour for the sample nodes is computed by subset DP.',
  'Optimal BST': 'Minimum weighted search cost = 180.',
  'DAG Shortest Path': 'Shortest distance from Emergency Center to Shelter = 11.',
  'Floyd-Warshall': 'All-pairs shortest distances are computed for the city network.',
  LIS: 'Longest increasing subsequence length = 4.',
  LCS: 'LCS = ASSESS → EVACUATE → SHELTER, length 3.',
  Independent: 'Largest independent set in the hierarchy has size 5.',
  Dijkstra: 'Industrial Area → Central Shelter = 40 km local preview.',
  MST: 'Kruskal selects 5 roads with total distance 102 km.',
  'Fractional Knapsack': 'Capacity 65 kg → maximum value 295.',
  Scheduling: '4 emergency tasks scheduled, total priority 300.',
  Huffman: 'DISASTER compresses from 64 bits to 22 bits in the demo.',
  Backtracking: 'Constraint search with pruning is demonstrated.',
  'Ford-Fulkerson': 'Maximum evacuation flow = 150 people.',
  'Max Flow': 'Edmonds-Karp maximum evacuation flow = 150 people.',
  'Min Cut': 'Minimum cut capacity = 150; equal to max flow.',
  'Bipartite Matching': '3 rescue teams matched to 3 emergencies.',
  SAT: 'Formula is satisfiable with A=FALSE, B=TRUE, C=FALSE.',
  '3-SAT': '3-CNF formula is satisfiable with A=FALSE, B=FALSE, C=TRUE.',
  Clique: 'Clique of size 3: Emergency Center, Industrial Area, City Hospital.',
  'Vertex Cover': 'Minimum cover demonstration selects 3 locations.',
  ILP: '0-1 ILP model finds a minimum objective value of 3.',
};

export function AlgorithmsPage() {
  const [query, setQuery] = useState('');
  const [unit, setUnit] = useState('All units');
  const [selected, setSelected] = useState<Algorithm | null>(null);
  const [demoRunning, setDemoRunning] = useState(false);
  const [demoOutput, setDemoOutput] = useState('');

  const shown = algorithms.filter(
    (a) =>
      (unit === 'All units' || a.unit === unit) &&
      a.name.toLowerCase().includes(query.toLowerCase())
  );

const runDemo = async (a: Algorithm) => {
  setDemoRunning(true);
  setDemoOutput('');

  try {
    // 1. Dijkstra - Flask backend
    if (a.name.toLowerCase() === 'dijkstra') {
      const response = await fetch(
        'http://127.0.0.1:5000/api/v1/evacuation/route',
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({
            source: 'L10',
            destination: 'L7',
          }),
        }
      );

      const data = await response.json();

      if (!response.ok || data.success === false) {
        throw new Error(
          data.error || data.message || 'Dijkstra API request failed'
        );
      }

      const routeNames = Array.isArray(data.names)
        ? data.names
        : Array.isArray(data.path_names)
          ? data.path_names
          : [];

      const route = routeNames.length
        ? routeNames.join(' → ')
        : Array.isArray(data.path)
          ? data.path.join(' → ')
          : 'Route unavailable';

      setDemoOutput(
        `BACKEND RESULT\n\n` +
        `Optimal Route: ${route}\n\n` +
        `Shortest Distance: ${data.distance} km\n\n` +
        `Estimated Travel Time: ${data.estimated_time} minutes\n\n` +
        `Algorithm: Dijkstra\n\n` +
        `Time Complexity: O((V + E) log V)`
      );

      return;
    }

    // 2. Coin Change - Flask backend
    if (a.name.toLowerCase() === 'coin change') {
      const response = await fetch(
        'http://127.0.0.1:5000/api/v1/algorithms/coin-change?amount=63&denominations=1,5,10,25'
      );

      const data = await response.json();

      if (!response.ok || data.success === false) {
        throw new Error(
          data.error || data.message || 'Coin Change API request failed'
        );
      }

      const result = data.result ?? data;
      const amount = result.amount ?? 63;

      const minimumCoins =
        result.coin_count ??
        result.minimum_coins ??
        result.min_coins ??
        result.count;

     const solution = Array.isArray(result.solution)
  ? result.solution
  : [];

const packageBreakdown = solution
  .map(
    (item: { count: number; denomination: number }) =>
      `${item.count} × ${item.denomination}`
  )
  .join(', ');

      setDemoOutput(
        `BACKEND RESULT\n\n` +
        `Amount: ${amount}\n\n` +
        `Minimum Packages: ${minimumCoins ?? 'Check API response'}\n\n` +
        `Denominations: 1, 5, 10, 25\n\n` +
      `Packages Used: ${packageBreakdown || 'Not available'}\n\n` +
`Time Complexity: ${result.time_complexity}\n\n` +
`Space Complexity: ${result.space_complexity}`
      );

      return;
    }

    // 3. Other algorithms - existing local demonstrations
  // 3. BFS - Flask backend
if (
  ['bfs', 'breadth-first search'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/graph/traverse',
    {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        algorithm: 'BFS',
        source: 'L10',
      }),
    }
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.error || data.message || 'BFS API request failed'
    );
  }

  const traversalNames = Array.isArray(data.path_names)
    ? data.path_names
    : Array.isArray(data.traversal)
      ? data.traversal
      : [];

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Breadth-First Search (BFS)\n\n` +
    `Starting Location: Industrial Area (L10)\n\n` +
    `Traversal Order: ${traversalNames.join(' → ')}\n\n` +
    `Visited Locations: ${data.visited_count ?? traversalNames.length}\n\n` +
    `Levels: ${
      Array.isArray(data.levels)
        ? data.levels.join(', ')
        : 'Not available'
    }\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'O(V + E)'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'O(V)'}`
  );

  return;
}

// 4. Other algorithms - existing local demonstrations
// 3. BFS - Flask backend
if (
  ['bfs', 'breadth-first search'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/graph/traverse',
    {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        algorithm: 'BFS',
        source: 'L10',
      }),
    }
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.error || data.message || 'BFS API request failed'
    );
  }

  const traversalNames = Array.isArray(data.path_names)
    ? data.path_names
    : Array.isArray(data.traversal)
      ? data.traversal
      : [];

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Breadth-First Search (BFS)\n\n` +
    `Starting Location: Industrial Area (L10)\n\n` +
    `Traversal Order: ${traversalNames.join(' → ')}\n\n` +
    `Visited Locations: ${data.visited_count ?? traversalNames.length}\n\n` +
    `Levels: ${
      Array.isArray(data.levels)
        ? data.levels.join(', ')
        : 'Not available'
    }\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'O(V + E)'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'O(V)'}`
  );

  return;
}

// 4. Other algorithms - existing local demonstrations
// 3. BFS - Flask backend
if (
  ['bfs', 'breadth-first search'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/graph/traverse',
    {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        algorithm: 'BFS',
        source: 'L10',
      }),
    }
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.error || data.message || 'BFS API request failed'
    );
  }

  const traversalNames = Array.isArray(data.path_names)
    ? data.path_names
    : Array.isArray(data.traversal)
      ? data.traversal
      : [];

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Breadth-First Search (BFS)\n\n` +
    `Starting Location: Industrial Area (L10)\n\n` +
    `Traversal Order: ${traversalNames.join(' → ')}\n\n` +
    `Visited Locations: ${data.visited_count ?? traversalNames.length}\n\n` +
    `Levels: ${
      Array.isArray(data.levels)
        ? data.levels.join(', ')
        : 'Not available'
    }\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'O(V + E)'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'O(V)'}`
  );

  return;
}

// 4. Other algorithms - existing local demonstrations
// 3. BFS - Flask backend
if (
  ['bfs', 'breadth-first search'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/graph/traverse',
    {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        algorithm: 'BFS',
        source: 'L10',
      }),
    }
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.error || data.message || 'BFS API request failed'
    );
  }

  const traversalNames = Array.isArray(data.path_names)
    ? data.path_names
    : Array.isArray(data.traversal)
      ? data.traversal
      : [];

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Breadth-First Search (BFS)\n\n` +
    `Starting Location: Industrial Area (L10)\n\n` +
    `Traversal Order: ${traversalNames.join(' → ')}\n\n` +
    `Visited Locations: ${data.visited_count ?? traversalNames.length}\n\n` +
    `Levels: ${
      Array.isArray(data.levels)
        ? data.levels.join(', ')
        : 'Not available'
    }\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'O(V + E)'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'O(V)'}`
  );

  return;
}
// DFS - Flask backend
if (
  ['dfs', 'depth-first search'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/graph/traverse',
    {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        algorithm: 'DFS',
        source: 'L10',
      }),
    }
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.error || data.message || 'DFS API request failed'
    );
  }

  const traversalNames = Array.isArray(data.path_names)
    ? data.path_names
    : Array.isArray(data.traversal)
      ? data.traversal
      : [];

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Depth-First Search (DFS)\n\n` +
    `Starting Location: Industrial Area (L10)\n\n` +
    `Traversal Order: ${traversalNames.join(' → ')}\n\n` +
    `Visited Locations: ${data.visited_count ?? traversalNames.length}\n\n` +
    `Time Complexity: O(V + E)\n\n` +
    `Space Complexity: O(V)`
  );

  return;
}
// Topological Sort - Flask backend
if (
  ['topological sort', 'topological sorting'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/topological-sort'
  );

  const data = await response.json();

  // Handle cycle detection
  if (response.status === 409 || data.cycle_detected) {
    setDemoOutput(
      `BACKEND RESULT\n\n` +
      `Algorithm: Topological Sort\n\n` +
      `Cycle Detected: Yes\n\n` +
      `A valid topological ordering is not possible.\n\n` +
      `Backend Response:\n${JSON.stringify(data, null, 2)}`
    );

    return;
  }

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Topological Sort API request failed'
    );
  }

  const order =
    data.topological_order ??
    data.order ??
    data.sorted_tasks ??
    data.task_order ??
    data.result;

 const orderText = Array.isArray(order)
  ? order
      .map((task: { task_id: string; task_name: string }) =>
        `${task.task_id}: ${task.task_name}`
      )
      .join('\n↓\n')
  : typeof order === 'string'
    ? order
    : 'No task ordering available';

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Topological Sort\n\n` +
    `Cycle Detected: No\n\n` +
    `Topological Order: ${orderText}\n\n` +
    `Time Complexity: O(V + E)\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Floyd-Warshall - Flask backend
if (
  a.name.toLowerCase().includes('floyd') &&
  a.name.toLowerCase().includes('warshall')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/graph/floyd-warshall?source=L10&destination=L7'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || data.error || 'Floyd-Warshall API request failed'
    );
  }

  const pathValue =
    data.path_names ??
    data.names ??
    data.path ??
    data.route;

  const pathText = Array.isArray(pathValue)
    ? pathValue
        .map((item: any) =>
          typeof item === 'string'
            ? item
            : item.name ?? item.location_name ?? item.id ?? JSON.stringify(item)
        )
        .join(' → ')
    : typeof pathValue === 'string'
      ? pathValue
      : 'Not available in response';

  const distance =
    data.distance ??
    data.shortest_distance ??
    data.total_distance ??
    data.distance_km ??
    data.cost;

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Floyd-Warshall\n\n` +
    `Source: Industrial Area (L10)\n\n` +
    `Destination: Central Shelter (L7)\n\n` +
    `Shortest Path: ${pathText}\n\n` +
    `Shortest Distance: ${
      distance !== undefined ? `${distance} km` : 'Check API response'
    }\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'O(V³)'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'O(V²)'}\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Minimum Spanning Tree - Flask backend
if (
  ['mst', 'minimum spanning tree'].includes(
    a.name.trim().toLowerCase()
  ) ||
  a.name.toLowerCase().includes('minimum spanning tree')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/graph/mst'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || data.error || 'MST API request failed'
    );
  }

  const edges =
    data.mst_edges ??
    data.edges ??
    data.selected_edges ??
    data.tree_edges ??
    data.result?.edges;

 const edgeText = Array.isArray(edges)
  ? edges.map((edge: any) =>
      typeof edge === 'string'
        ? edge
        : `${edge.source ?? edge.from ?? edge.u ?? '?'} → ${edge.destination ?? edge.to ?? edge.target ?? edge.v ?? '?'} (Weight: ${edge.distance ?? edge.weight ?? edge.cost ?? '?'}) [${edge.road_id ?? 'Road'}]`
    ).join('\n')
  : 'See backend response below';

  const totalWeight =
    data.total_weight ??
    data.total_cost ??
    data.total_distance ??
    data.weight ??
    data.cost;

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Minimum Spanning Tree (MST)\n\n` +
    `Selected Edges:\n${edgeText}\n\n` +
    `Total MST Weight: ${totalWeight ?? 'Not available'}\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'O(E log V)'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'O(V)'}\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Fractional Knapsack - Flask backend
if (
  a.name.toLowerCase().includes('knapsack') ||
  a.name.toLowerCase().includes('fractional')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/resources/allocate?resource_type=WATER&capacity=1200'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || data.error || 'Fractional Knapsack API request failed'
    );
  }

  const selectedResources = Array.isArray(data.selected_resources)
    ? data.selected_resources
    : [];

  const allocationDetails = selectedResources.length
    ? selectedResources.map(
        (item: {
          resource_id: string;
          location_id: string;
          type: string;
          unit: string;
          allocated_quantity: number;
          allocated_value: number;
        }) =>
          `${item.resource_id} | ${item.type} | ${item.location_id} | ` +
          `${item.allocated_quantity} ${item.unit} | Value: ${item.allocated_value}`
      ).join('\n')
    : 'No allocation details available';

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Fractional Knapsack\n\n` +
    `Resource Type: ${data.resource_type ?? 'WATER'}\n\n` +
    `Capacity: ${data.capacity ?? 1200}\n\n` +
    `Selected Resources:\n${allocationDetails}\n\n` +
    `Total Allocated Quantity: ${data.allocated_quantity ?? 'Not available'}\n\n` +
    `Remaining Capacity: ${data.remaining_capacity ?? 'Not available'}\n\n` +
    `Total Priority Value: ${data.total_priority_value ?? 'Not available'}\n\n` +
    `Time Complexity: O(n log n)\n\n` +
    `Space Complexity: O(n)`
  );

  return;
}
// Scheduling - Flask backend
if (a.name.trim().toLowerCase().includes('scheduling')) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/scheduling'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || data.error || 'Scheduling API request failed'
    );
  }

  const schedule =
    data.scheduled_jobs ??
    data.schedule ??
    data.jobs ??
    data.order ??
    data.result;

 const scheduleText = Array.isArray(schedule)
  ? schedule
      .map((job: any, index: number) =>
        `Slot ${job.slot ?? index + 1}: ` +
        `${job.job_name ?? job.job_id ?? 'Unknown Job'} | ` +
        `Deadline: ${job.deadline ?? 'N/A'} | ` +
        `Priority: ${job.priority_value ?? 'N/A'}`
      )
      .join('\n')
  : typeof schedule === 'string'
    ? schedule
    : 'Schedule format not recognized';

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm ?? 'Job Scheduling'}\n\n` +
    `Scheduled Jobs:\n${scheduleText}\n\n` +
    `Total Jobs: ${
      data.job_count ??
      data.total_jobs ??
      (Array.isArray(schedule) ? schedule.length : 'Not available')
    }\n\n` +
    `Total Waiting Time: ${data.total_waiting_time ?? 'Not provided'}\n\n` +
    `Total Turnaround Time: ${data.total_turnaround_time ?? 'Not provided'}\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'See backend response'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'See backend response'}\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Maximum Flow - Flask backend
if (
  a.name.toLowerCase().includes('max flow') ||
  a.name.toLowerCase().includes('maximum flow')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/flow/max?source=L10&destination=L7'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || data.error || 'Maximum Flow API request failed'
    );
  }

  const result = data.result ?? data;

  const maximumFlow =
    result.max_flow ??
    result.maximum_flow ??
    result.maxFlow ??
    result.flow_value ??
    result.total_flow ??
    result.value;

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${result.algorithm ?? 'Maximum Flow'}\n\n` +
    `Source: ${result.source ?? 'Industrial Area (L10)'}\n\n` +
    `Destination: ${result.destination ?? 'Central Shelter (L7)'}\n\n` +
    `Maximum Flow: ${maximumFlow ?? 'See backend response'}\n\n` +
    `Time Complexity: ${result.time_complexity ?? 'See backend response'}\n\n` +
    `Space Complexity: ${result.space_complexity ?? 'See backend response'}\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Minimum Cut - Flask backend
if (
  ['min cut', 'minimum cut', 'min-cut', 'minimum-cut'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/flow/min-cut?source=L10&destination=L7'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || data.error || 'Minimum Cut API request failed'
    );
  }

  const cutEdges =
    data.cut_edges ??
    data.min_cut_edges ??
    data.edges ??
    data.result?.cut_edges ??
    [];

  const cutDetails = Array.isArray(cutEdges)
    ? cutEdges.map((edge: any, index: number) => {
        if (typeof edge === 'string') {
          return `${index + 1}. ${edge}`;
        }

        return (
          `${index + 1}. ${edge.source ?? edge.from ?? '?'} → ` +
          `${edge.destination ?? edge.to ?? '?'} | ` +
          `Capacity: ${edge.capacity ?? edge.weight ?? 'N/A'} | ` +
          `Road: ${edge.road_id ?? 'N/A'}`
        );
      }).join('\n')
    : 'Cut details not available';

  const cutCapacity =
    data.cut_capacity ??
    data.minimum_cut_capacity ??
    data.min_cut_capacity ??
    data.total_cut_capacity ??
    data.capacity;

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm ?? 'Minimum Cut'}\n\n` +
    `Source: ${data.source ?? 'Industrial Area (L10)'}\n\n` +
    `Destination: ${data.destination ?? 'Central Shelter (L7)'}\n\n` +
    `Minimum Cut Capacity: ${cutCapacity ?? 'See backend response'}\n\n` +
    `Cut Edges:\n${cutDetails || 'No cut edges returned'}\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'See backend response'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'See backend response'}\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Bipartite Matching - Flask backend
if (
  a.name.toLowerCase().includes('matching') ||
  a.name.toLowerCase().includes('bipartite')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/rescue-teams/match'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || data.error || 'Bipartite Matching API request failed'
    );
  }

  const assignments =
    data.assignments ??
    data.matches ??
    data.matching ??
    data.matched_pairs ??
    data.result?.assignments ??
    data.result?.matches ??
    [];

  const assignmentText = Array.isArray(assignments)
    ? assignments.map((item: any, index: number) => {
        if (typeof item !== 'object' || item === null) {
          return `${index + 1}. ${String(item)}`;
        }

        const emergency =
          item.emergency_name ??
          item.emergency_id ??
          item.emergency ??
          item.left ??
          'Emergency';

        const team =
          item.team_name ??
          item.rescue_team_name ??
          item.team_id ??
          item.team ??
          item.right ??
          'Unassigned';

        return `${index + 1}. ${emergency} → ${team}`;
      }).join('\n')
    : 'Assignment format not recognized';

  const matchedCount =
    data.matched_count ??
    data.total_matches ??
    data.match_count ??
    (Array.isArray(assignments) ? assignments.length : 'Not available');

  const unmatched =
    data.unmatched_emergencies ??
    data.unmatched ??
    [];

  const unmatchedText = Array.isArray(unmatched)
    ? unmatched.map((item: any) =>
        typeof item === 'object'
          ? item.emergency_name ?? item.emergency_id ?? item.id ?? JSON.stringify(item)
          : String(item)
      ).join(', ') || 'None'
    : String(unmatched);

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Bipartite Matching\n\n` +
    `Rescue Team Assignments:\n${assignmentText}\n\n` +
    `Successful Matches: ${matchedCount}\n\n` +
    `Available Teams: ${data.available_teams ?? 'See backend response'}\n\n` +
    `Unmatched Emergencies: ${unmatchedText}\n\n` +
    `Time Complexity: ${data.time_complexity ?? 'See backend response'}\n\n` +
    `Space Complexity: ${data.space_complexity ?? 'See backend response'}\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Travelling Salesman Problem - Flask backend
if (
  ['tsp', 'travelling salesman problem', 'traveling salesman problem'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/tsp?start=L8'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || data.error || 'TSP API request failed'
    );
  }

  const result = data.result ?? data;

  const tour =
    result.tour ??
    result.optimal_route ??
    result.route ??
    result.path ??
    result.visited_order;

  const tourText = Array.isArray(tour)
    ? tour
        .map((item: any) =>
          typeof item === 'string'
            ? item
            : item.location_name ?? item.name ?? item.location_id ?? item.id ?? JSON.stringify(item)
        )
        .join(' → ')
    : typeof tour === 'string'
      ? tour
      : 'Tour not available in response';

  const distance =
    result.total_distance ??
    result.minimum_distance ??
    result.shortest_distance ??
    result.distance ??
    result.total_cost ??
    result.min_cost;

  const noTour =
    result.reachable === false ||
    result.tour_found === false ||
    result.success === false ||
    (typeof distance === 'number' && distance >= 999999);

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${result.algorithm ?? 'Travelling Salesman Problem'}\n\n` +
    `Starting Location: ${result.start ?? 'Emergency Center (L8)'}\n\n` +
    `Optimal Tour: ${noTour ? 'No valid tour found' : tourText}\n\n` +
    `Total Distance: ${
      typeof distance === 'number' && distance < 999999
        ? `${distance} km`
        : 'Not available'
    }\n\n` +
    `Time Complexity: ${result.time_complexity ?? 'See backend response'}\n\n` +
    `Space Complexity: ${result.space_complexity ?? 'See backend response'}\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Optimal BST - Flask backend
if (
  a.name.toLowerCase().includes('optimal bst') ||
  a.name.toLowerCase().includes('optimal binary search tree')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/optimal-bst'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Optimal BST API request failed'
    );
  }

  const locationDetails = Array.isArray(data.locations)
    ? data.locations
        .map(
          (item: { name: string; frequency: number }) =>
            `${item.name}: ${item.frequency}`
        )
        .join('\n')
    : 'Not available';

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Search Frequencies:\n${locationDetails}\n\n` +
    `Minimum Weighted Search Cost: ${data.minimum_weighted_search_cost}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Emergency Merge Sort - Flask backend
if (
  a.unit === 'UNIT I' &&
  ['emergency merge sort', 'emergency sort', 'sorting'].includes(
    a.name.trim().toLowerCase()
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/lab/run',
    {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        algorithm: 'emergency_sort',
        order: 'DESC',
      }),
    }
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Emergency Merge Sort API failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Emergency Merge Sort\n\n` +
    `Sorting Order: Descending Priority\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Emergency Linear Search - Flask backend
if (
  ['searching', 'search', 'linear search', 'emergency linear search']
    .includes(a.name.trim().toLowerCase())
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/lab/run',
    {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        algorithm: 'emergency_search',
        query: 'FIRE',
      }),
    }
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Emergency Linear Search API failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: Emergency Linear Search\n\n` +
    `Search Query: FIRE\n\n` +
    `Backend Response:\n${JSON.stringify(data, null, 2)}`
  );

  return;
}
// Recursion - Flask backend
if (a.name.trim().toLowerCase().includes('recursion')) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/recursion'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Recursion API request failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Root: ${data.root}\n\n` +
    `Disaster Zone Traversal:\n\n${data.output}\n\n` +
    `Total Locations: ${data.node_count}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Merge Sort - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase() === 'merge sort'
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/merge-sort'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Merge Sort API request failed'
    );
  }

  const sortedRecords = Array.isArray(data.emergencies)
    ? data.emergencies
        .map(
          (item: {
            id: string;
            type: string;
            location: string;
            affected_people: number;
          }, index: number) =>
            `${index + 1}. ${item.id} | ${item.type} | ` +
            `${item.location} | ${item.affected_people} people`
        )
        .join('\n')
    : 'No sorted records available';

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Sorting Order: Highest to Lowest\n\n` +
    `Sorted Emergencies:\n${sortedRecords}\n\n` +
    `Total Records: ${data.count}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Quick Sort - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase() === 'quick sort'
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/quick-sort'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Quick Sort API request failed'
    );
  }

  const sortedRecords = Array.isArray(data.emergencies)
    ? data.emergencies
        .map(
          (item: {
            id: string;
            type: string;
            location: string;
            affected_people: number;
          }, index: number) =>
            `${index + 1}. ${item.id} | ${item.type} | ` +
            `${item.location} | ${item.affected_people} people`
        )
        .join('\n')
    : 'No sorted records available';

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Pivot Selection: ${data.pivot}\n\n` +
    `Sorting Order: Highest to Lowest\n\n` +
    `Sorted Emergencies:\n${sortedRecords}\n\n` +
    `Total Records: ${data.count}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Closest Pair - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase() === 'closest pair'
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/closest-pair'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Closest Pair API request failed'
    );
  }

  const pointsText = Array.isArray(data.points)
    ? data.points
        .map(
          (point: { name: string; x: number; y: number }) =>
            `${point.name}: (${point.x}, ${point.y})`
        )
        .join('\n')
    : 'Not available';

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Emergency Coordinates:\n${pointsText}\n\n` +
    `Closest Pair:\n${data.closest_pair.join(' ↔ ')}\n\n` +
    `Minimum Distance: ${data.minimum_distance} ${data.distance_unit}\n\n` +
    `Total Locations: ${data.point_count}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Majority Element - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase() === 'majority element'
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/majority-element'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Majority Element API request failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Majority Exists: ${data.has_majority ? 'Yes' : 'No'}\n\n` +
    `Majority Emergency Type: ${data.majority_type}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}\n\n` +
    `C++ Execution Output:\n${data.output}`
  );

  return;
}
// Unit II Order Statistics - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase() === 'order statistics'
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/order-statistics'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Order Statistics API request failed'
    );
  }

  const emergency = data.emergency;

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Rank: ${data.rank}\n\n` +
    `Emergency ID: ${emergency.id}\n\n` +
    `Emergency Type: ${emergency.type}\n\n` +
    `Location: ${emergency.location}\n\n` +
    `Affected People: ${emergency.affected_people}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Euclid's Algorithm - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase().includes('euclid')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/euclid'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Euclid API request failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Water Quantity: ${data.water_units} units\n\n` +
    `Food Quantity: ${data.food_units} units\n\n` +
    `Greatest Common Divisor: ${data.gcd}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Karatsuba Multiplication - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase().includes('karatsuba')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/karatsuba'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Karatsuba API request failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Number 1: ${data.number1}\n\n` +
    `Number 2: ${data.number2}\n\n` +
    `Multiplication Result: ${data.result.toLocaleString('en-IN')}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Randomized Quick Sort - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase().includes('randomized')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/randomized'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Randomized Quick Sort API request failed'
    );
  }

  const emergencyList = data.sorted_emergencies
    .map(
      (emergency: {
        id: string;
        type: string;
        location: string;
        affected_people: number;
      }, index: number) =>
        `${index + 1}. ${emergency.id} | ${emergency.type} | ` +
        `${emergency.location} | ${emergency.affected_people} people`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Pivot Strategy: ${data.pivot_strategy}\n\n` +
    `Sorting Order: ${data.sort_order}\n\n` +
    `Sorted Emergencies:\n${emergencyList}\n\n` +
    `Highest Priority: ${data.highest_priority.id} - ` +
    `${data.highest_priority.type}\n` +
    `Location: ${data.highest_priority.location}\n` +
    `Affected People: ${data.highest_priority.affected_people}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Josephus Problem - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase().includes('josephus')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/josephus'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Josephus API request failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Number of Teams: ${data.number_of_teams}\n\n` +
    `Elimination Step: ${data.elimination_step}\n\n` +
    `Surviving Team: Team ${data.surviving_team}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Fast Modular Exponentiation - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase().includes('fast modular exponentiation')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/fast-modular-exponentiation'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Fast Modular Exponentiation API request failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Calculation: ${data.base}^${data.exponent} mod ${data.modulus}\n\n` +
    `Result: ${data.result}\n\n` +
    `Method: Repeated Squaring\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Tower of Hanoi - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase().includes('tower of hanoi')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/tower-of-hanoi'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Tower of Hanoi API request failed'
    );
  }

  const moveList = data.moves
    .map(
      (
        move: {
          disk: number;
          source: string;
          destination: string;
        },
        index: number
      ) =>
        `${index + 1}. Move disk ${move.disk} from ` +
        `${move.source} to ${move.destination}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Number of Disks: ${data.number_of_disks}\n\n` +
    `Source Rod: ${data.rods.source}\n` +
    `Auxiliary Rod: ${data.rods.auxiliary}\n` +
    `Destination Rod: ${data.rods.destination}\n\n` +
    `Steps to Move All Disks:\n${moveList}\n\n` +
    `Total Moves Required: ${data.total_moves}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit II Inversion Count - Flask backend
if (
  a.unit === 'UNIT II' &&
  a.name.trim().toLowerCase().includes('inversion')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/inversion-count'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Inversion Count API request failed'
    );
  }

  const originalList = data.original_emergencies
    .map(
      (
        emergency: { id: string; affected_people: number },
        index: number
      ) =>
        `${index + 1}. ${emergency.id} - ${emergency.affected_people} people`
    )
    .join('\n');

  const sortedList = data.sorted_emergencies
    .map(
      (
        emergency: { id: string; affected_people: number },
        index: number
      ) =>
        `${index + 1}. ${emergency.id} - ${emergency.affected_people} people`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Original Emergency Priority List:\n${originalList}\n\n` +
    `Sorted Emergency List:\n${sortedList}\n\n` +
    `Total Inversions: ${data.inversion_count}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit III Fibonacci - Flask backend
if (
  a.unit === 'UNIT III' &&
  a.name.trim().toLowerCase().includes('fibonacci')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/fibonacci'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Fibonacci API request failed'
    );
  }

  const dpTable = data.dp_table
    .map(
      (item: { index: number; value: number }) =>
        `F[${item.index}] = ${item.value}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Input: n = ${data.n}\n\n` +
    `Fibonacci(${data.n}) = ${data.fibonacci_value}\n\n` +
    `DP Table:\n${dpTable}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit III Binomial Coefficient - Flask backend
if (
  a.unit === 'UNIT III' &&
  a.name.trim().toLowerCase().includes('binomial')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/binomial'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Binomial Coefficient API request failed'
    );
  }

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Available Response Teams: ${data.available_teams}\n\n` +
    `Teams to Select: ${data.teams_to_select}\n\n` +
    `Binomial Coefficient: C(${data.available_teams}, ${data.teams_to_select}) = ${data.binomial_coefficient}\n\n` +
    `Interpretation: ${data.binomial_coefficient} possible ways to select ${data.teams_to_select} teams from ${data.available_teams} available teams.\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit III Matrix Chain Multiplication - Flask backend
if (
  a.unit === 'UNIT III' &&
  a.name.trim().toLowerCase().includes('matrix chain')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/matrix-chain'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Matrix Chain API request failed'
    );
  }

  const matrixDetails = data.matrices
    .map(
      (matrix: {
        matrix: string;
        rows: number;
        columns: number;
      }) =>
        `${matrix.matrix}: ${matrix.rows} x ${matrix.columns}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Number of Matrices: ${data.matrix_count}\n\n` +
    `Matrix Dimensions:\n${matrixDetails}\n\n` +
    `Minimum Scalar Multiplications: ${data.minimum_scalar_multiplications.toLocaleString('en-IN')}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit III DAG Shortest Path - Flask backend
if (
  a.unit === 'UNIT III' &&
  a.name.trim().toLowerCase().includes('dag shortest path')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/dag-shortest-path'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'DAG Shortest Path API request failed'
    );
  }

  const distanceList = data.distances
    .map(
      (
        item: {
          location: string;
          distance: number | null;
          reachable: boolean;
        },
        index: number
      ) =>
        `${index + 1}. ${item.location} → ${
          item.reachable ? `${item.distance} units` : 'Unreachable'
        }`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Starting Location: ${data.source}\n\n` +
    `Destination: ${data.destination}\n\n` +
    `Shortest Distances:\n${distanceList}\n\n` +
    `Shortest Distance to Shelter: ${data.shortest_distance_to_shelter} units\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit III Longest Increasing Subsequence - Flask backend
if (
  a.unit === 'UNIT III' &&
  (
    a.name.trim().toLowerCase().includes('longest increasing subsequence') ||
    a.name.trim().toLowerCase() === 'lis'
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/lis'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'LIS API request failed'
    );
  }

  const priorityList = data.priority_sequence.join(', ');

  const dpTable = data.dp_table
    .map(
      (item: { priority: number; lis_length: number }) =>
        `Priority ${item.priority} → LIS length = ${item.lis_length}`
    )
    .join('\n');

  const lisSequence = data.lis_sequence.join(' → ');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Original Priority Sequence: ${priorityList}\n\n` +
    `DP Table:\n${dpTable}\n\n` +
    `Longest Increasing Subsequence: ${lisSequence}\n\n` +
    `LIS Length: ${data.lis_length}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit III Longest Common Subsequence - Flask backend
if (
  a.unit === 'UNIT III' &&
  (
    a.name.trim().toLowerCase().includes('longest common subsequence') ||
    a.name.trim().toLowerCase() === 'lcs'
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/lcs'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'LCS API request failed'
    );
  }

  const sequence1 = data.sequence1.join(' → ');
  const sequence2 = data.sequence2.join(' → ');
  const lcsSequence = data.lcs_sequence.join(' → ');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Response Sequence 1:\n${sequence1}\n\n` +
    `Response Sequence 2:\n${sequence2}\n\n` +
    `Longest Common Subsequence:\n${lcsSequence}\n\n` +
    `LCS Length: ${data.lcs_length}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit III Independent Set - Flask backend
if (
  a.unit === 'UNIT III' &&
  a.name.trim().toLowerCase().includes('independent set')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/independent-set'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Independent Set API request failed'
    );
  }

  const selectedLocations = data.selected_locations
    .map(
      (location: string, index: number) =>
        `${index + 1}. ${location}`
    )
    .join('\n');

  const connectionList = data.connections
    .map(
      (connection: {
        source: string;
        destination: string;
      }) =>
        `${connection.source} → ${connection.destination}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Total Locations: ${data.location_count}\n\n` +
    `Tree Connections:\n${connectionList}\n\n` +
    `Maximum Independent Set Size: ${data.maximum_independent_set_size}\n\n` +
    `Selected Locations:\n${selectedLocations}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit IV Huffman Coding - Flask backend
if (
  a.unit === 'UNIT IV' &&
  a.name.trim().toLowerCase().includes('huffman')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/huffman'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Huffman Coding API request failed'
    );
  }

  const frequencyList = data.character_frequencies
    .map(
      (item: { character: string; frequency: number }) =>
        `${item.character} → ${item.frequency}`
    )
    .join('\n');

  const codeList = data.huffman_codes
    .map(
      (item: { character: string; code: string }) =>
        `${item.character} → ${item.code}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Original Report: ${data.report_text}\n\n` +
    `Character Frequencies:\n${frequencyList}\n\n` +
    `Huffman Codes:\n${codeList}\n\n` +
    `Original Size: ${data.original_bits} bits\n\n` +
    `Compressed Size: ${data.compressed_bits} bits\n\n` +
    `Encoded Report:\n${data.encoded_report}\n\n` +
    `Bits Saved: ${data.bits_saved}\n\n` +
    `Space Saving: ${data.space_saving_percentage}%\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit IV Graph Coloring - Flask backend
if (
  a.unit === 'UNIT IV' &&
  a.name.trim().toLowerCase().includes('graph coloring')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/graph-coloring'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Graph Coloring API request failed'
    );
  }

  const assignments = data.color_assignments
    .map(
      (
        item: { location: string; color: number },
        index: number
      ) =>
        `${index + 1}. ${item.location} → Color ${item.color}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Total Locations: ${data.location_count}\n\n` +
    `Available Colors: ${data.total_colors}\n\n` +
    `Coloring Possible: ${data.coloring_possible ? 'Yes' : 'No'}\n\n` +
    `Color Assignments:\n${assignments}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit IV N-Queens - Flask backend
if (
  a.unit === 'UNIT IV' &&
  (
    a.name.trim().toLowerCase().includes('n-queens') ||
    a.name.trim().toLowerCase().includes('n queens')
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/n-queens'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'N-Queens API request failed'
    );
  }

  const boardText = data.board
    .map(
      (row: string[], index: number) =>
        `Row ${index + 1}: ${row.join(' ')}`
    )
    .join('\n');

  const queenPositions = data.queen_positions
    .map(
      (
        queen: { row: number; column: number },
        index: number
      ) =>
        `${index + 1}. Row ${queen.row}, Column ${queen.column}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Board Size: ${data.board_size} x ${data.board_size}\n\n` +
    `Number of Queens: ${data.number_of_queens}\n\n` +
    `Solution Found: ${data.solution_found ? 'Yes' : 'No'}\n\n` +
    `N-Queens Board:\n${boardText}\n\n` +
    `Queen Positions:\n${queenPositions}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit IV Hamiltonian Cycle - Flask backend
if (
  a.unit === 'UNIT IV' &&
  a.name.trim().toLowerCase().includes('hamiltonian')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/hamiltonian-cycle'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Hamiltonian Cycle API request failed'
    );
  }

  const cycleText = data.cycle_found
    ? data.cycle.join(' → ')
    : 'No Hamiltonian Cycle exists.';

  const locationList = data.locations
    .map(
      (location: { id: number; name: string }) =>
        `${location.id}. ${location.name}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Total Locations: ${data.location_count}\n\n` +
    `Starting Location: ${data.starting_location}\n\n` +
    `Selected Locations:\n${locationList}\n\n` +
    `Hamiltonian Cycle Found: ${data.cycle_found ? 'Yes' : 'No'}\n\n` +
    `Hamiltonian Cycle:\n${cycleText}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit IV Subset Sum - Flask backend
if (
  a.unit === 'UNIT IV' &&
  a.name.trim().toLowerCase().includes('subset sum')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/subset-sum'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Subset Sum API request failed'
    );
  }

  const resources = data.resources.join(', ');
  const selectedResources = data.selected_resources.join(' + ');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Available Resources: ${resources}\n\n` +
    `Required Target: ${data.target}\n\n` +
    `Subset Found: ${data.subset_found ? 'Yes' : 'No'}\n\n` +
    `Selected Resources: ${selectedResources}\n\n` +
    `Subset Sum: ${data.subset_sum}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit IV Sudoku Solver - Flask backend
if (
  a.unit === 'UNIT IV' &&
  a.name.trim().toLowerCase().includes('sudoku')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/sudoku'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Sudoku API request failed'
    );
  }

  const formatBoard = (board: number[][]) =>
    board
      .map((row: number[], index: number) =>
        `Row ${index + 1}: ` +
        row.map((value: number) => value === 0 ? '.' : value).join(' ')
      )
      .join('\n');

  const initialBoard = formatBoard(data.initial_board);

  const solvedBoard = data.solution_found
    ? formatBoard(data.solved_board)
    : 'No solution exists for this Sudoku.';

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Board Size: ${data.board_size} x ${data.board_size}\n\n` +
    `Initial Sudoku Board:\n${initialBoard}\n\n` +
    `Solution Found: ${data.solution_found ? 'Yes' : 'No'}\n\n` +
    `Solved Sudoku Board:\n${solvedBoard}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit V Ford-Fulkerson - Flask backend
if (
  a.unit === 'UNIT V' &&
  a.name.trim().toLowerCase().includes('ford-fulkerson')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/ford-fulkerson'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Ford-Fulkerson API request failed'
    );
  }

  const edgeList = data.edges
    .map(
      (
        edge: {
          source: string;
          destination: string;
          capacity: number;
        },
        index: number
      ) =>
        `${index + 1}. ${edge.source} → ${edge.destination} | Capacity: ${edge.capacity}`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Source: ${data.source}\n\n` +
    `Destination: ${data.destination}\n\n` +
    `Total Locations: ${data.location_count}\n\n` +
    `Network Connections:\n${edgeList}\n\n` +
    `Maximum Evacuation Flow: ${data.maximum_flow} ${data.flow_unit}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit VI SAT - Flask backend
if (
  a.unit === 'UNIT VI' &&
  (
    a.name.trim().toLowerCase() === 'sat' ||
    a.name.trim().toLowerCase() === 'sat problem' ||
    a.name.trim().toLowerCase() === 'boolean sat'
  )
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/sat'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'SAT API request failed'
    );
  }

  const variableDetails = data.variables
    .map(
      (item: { variable: string; meaning: string }) =>
        `${item.variable} = ${item.meaning}`
    )
    .join('\n');

  const assignments = data.variables
    .map(
      (item: { variable: string; meaning: string }) =>
        `${item.variable} = ${
          data.satisfying_assignment[item.variable]
            ? 'TRUE'
            : 'FALSE'
        }`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Number of Variables: ${data.number_of_variables}\n\n` +
    `Boolean Variables:\n${variableDetails}\n\n` +
    `Boolean Formula:\n${data.formula}\n\n` +
    `Satisfying Assignment:\n${assignments}\n\n` +
    `Result: ${data.result}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
// Unit VI Circuit-SAT - Flask backend
if (
  a.unit === 'UNIT VI' &&
  a.name.trim().toLowerCase().includes('circuit') &&
  a.name.trim().toLowerCase().includes('sat')
) {
  const response = await fetch(
    'http://127.0.0.1:5000/api/v1/algorithms/circuit-sat'
  );

  const data = await response.json();

  if (!response.ok || data.success === false) {
    throw new Error(
      data.message || 'Circuit-SAT API request failed'
    );
  }

  const variableDetails = data.variables
    .map(
      (item: { variable: string; meaning: string }) =>
        `${item.variable} = ${item.meaning}`
    )
    .join('\n');

  const assignments = data.variables
    .map(
      (item: { variable: string; meaning: string }) =>
        `${item.variable} = ${
          data.satisfying_assignment[item.variable]
            ? 'TRUE'
            : 'FALSE'
        }`
    )
    .join('\n');

  setDemoOutput(
    `BACKEND RESULT\n\n` +
    `Algorithm: ${data.algorithm}\n\n` +
    `Method: ${data.method}\n\n` +
    `Number of Inputs: ${data.number_of_inputs}\n\n` +
    `Circuit Inputs:\n${variableDetails}\n\n` +
    `Circuit Formula:\n${data.formula}\n\n` +
    `Satisfying Assignment:\n${assignments}\n\n` +
    `OR Gate Output: ${data.or_gate_output ? 'TRUE' : 'FALSE'}\n\n` +
    `Final Circuit Output: ${data.final_circuit_output ? 'TRUE' : 'FALSE'}\n\n` +
    `Result: ${data.result}\n\n` +
    `Time Complexity: ${data.time_complexity}\n\n` +
    `Space Complexity: ${data.space_complexity}`
  );

  return;
}
        // Unit VI 3-SAT - Flask backend
        if (
          a.unit === 'UNIT VI' &&
          (
            a.name.trim().toLowerCase() === '3-sat' ||
            a.name.trim().toLowerCase() === '3-sat problem' ||
            a.name.trim().toLowerCase().includes('3-sat')
          )
        ) {
          const response = await fetch(
            'http://127.0.0.1:5000/api/v1/algorithms/three-sat'
          );

          const data = await response.json();

          if (!response.ok || data.success === false) {
            throw new Error(
              data.message || '3-SAT API request failed'
            );
          }

          const variableDetails = data.variables
            .map(
              (item: { variable: string; meaning: string }) =>
                `${item.variable} = ${item.meaning}`
            )
            .join('\n');

          const assignments = data.variables
            .map(
              (item: { variable: string; meaning: string }) =>
                `${item.variable} = ${
                  data.satisfying_assignment[item.variable]
                    ? 'TRUE'
                    : 'FALSE'
                }`
            )
            .join('\n');

          const clauseDetails = data.clauses
            .map(
              (clause: {
                number: number;
                expression: string;
                value: boolean;
              }) =>
                `Clause ${clause.number}: ${clause.expression} = ${
                  clause.value ? 'TRUE' : 'FALSE'
                }`
            )
            .join('\n');

          setDemoOutput(
            `BACKEND RESULT\n\n` +
            `Algorithm: ${data.algorithm}\n\n` +
            `Method: ${data.method}\n\n` +
            `Number of Variables: ${data.number_of_variables}\n\n` +
            `Number of Clauses: ${data.number_of_clauses}\n\n` +
            `Boolean Variables:\n${variableDetails}\n\n` +
            `3-CNF Formula:\n${data.formula}\n\n` +
            `Satisfying Assignment:\n${assignments}\n\n` +
            `Clause Evaluation:\n${clauseDetails}\n\n` +
            `All Clauses TRUE: ${
              data.all_clauses_true ? 'YES' : 'NO'
            }\n\n` +
            `Result: ${data.result}\n\n` +
            `Time Complexity: ${data.time_complexity}\n\n` +
            `Space Complexity: ${data.space_complexity}`
          );

          return;
        }
                // Unit VI Clique - Flask backend
        if (
          a.unit === 'UNIT VI' &&
          a.name.trim().toLowerCase().includes('clique')
        ) {
          const response = await fetch(
            'http://127.0.0.1:5000/api/v1/algorithms/clique'
          );

          const data = await response.json();

          if (!response.ok || data.success === false) {
            throw new Error(
              data.message || 'Clique API request failed'
            );
          }

          const locationDetails = data.locations
            .map(
              (location: string, index: number) =>
                `${index + 1}. ${location}`
            )
            .join('\n');

          const selectedLocations =
            data.selected_locations.length > 0
              ? data.selected_locations
                  .map((location: string) => `- ${location}`)
                  .join('\n')
              : 'No clique found for the required size.';

          setDemoOutput(
            `BACKEND RESULT\n\n` +
            `Algorithm: ${data.algorithm}\n\n` +
            `Method: ${data.method}\n\n` +
            `Number of Vertices: ${data.number_of_vertices}\n\n` +
            `Available Locations:\n${locationDetails}\n\n` +
            `Required Clique Size: ${data.required_clique_size}\n\n` +
            `Clique Found: ${data.clique_found ? 'YES' : 'NO'}\n\n` +
            `Selected Locations:\n${selectedLocations}\n\n` +
            `Clique Size: ${data.clique_size}\n\n` +
            `Clique Verified: ${data.is_clique_verified ? 'YES' : 'NO'}\n\n` +
            `Verification:\n${data.verification}\n\n` +
            `Result: ${data.result}\n\n` +
            `Time Complexity: ${data.time_complexity}\n\n` +
            `Space Complexity: ${data.space_complexity}`
          );

          return;
        }
                // Unit VI Vertex Cover - Flask backend
        if (
          a.unit === 'UNIT VI' &&
          a.name.trim().toLowerCase().includes('vertex') &&
          a.name.trim().toLowerCase().includes('cover')
        ) {
          const response = await fetch(
            'http://127.0.0.1:5000/api/v1/algorithms/vertex-cover'
          );

          const data = await response.json();

          if (!response.ok || data.success === false) {
            throw new Error(
              data.message || 'Vertex Cover API request failed'
            );
          }

          const locationDetails = data.locations
            .map(
              (location: string, index: number) =>
                `${index + 1}. ${location}`
            )
            .join('\n');

          const edgeDetails = data.edges
            .map(
              (edge: { source: string; destination: string }) =>
                `${edge.source} <-> ${edge.destination}`
            )
            .join('\n');

          const selectedLocations =
            data.selected_locations.length > 0
              ? data.selected_locations
                  .map((location: string) => `- ${location}`)
                  .join('\n')
              : 'No vertex cover found.';

          setDemoOutput(
            `BACKEND RESULT\n\n` +
            `Algorithm: ${data.algorithm}\n\n` +
            `Method: ${data.method}\n\n` +
            `Number of Vertices: ${data.number_of_vertices}\n\n` +
            `Available Locations:\n${locationDetails}\n\n` +
            `Number of Road Connections: ${data.number_of_edges}\n\n` +
            `Road Connections:\n${edgeDetails}\n\n` +
            `Required Vertex Cover Size: ${data.required_vertex_cover_size}\n\n` +
            `Vertex Cover Found: ${data.vertex_cover_found ? 'YES' : 'NO'}\n\n` +
            `Selected Locations:\n${selectedLocations}\n\n` +
            `Vertex Cover Size: ${data.vertex_cover_size}\n\n` +
            `All Edges Covered: ${data.all_edges_covered ? 'YES' : 'NO'}\n\n` +
            `Vertex Cover Verified: ${data.is_vertex_cover_verified ? 'YES' : 'NO'}\n\n` +
            `Verification:\n${data.verification}\n\n` +
            `Result: ${data.result}\n\n` +
            `Time Complexity: ${data.time_complexity}\n\n` +
            `Space Complexity: ${data.space_complexity}`
          );

          return;
        }
                // Unit VI ILP - Flask backend
        if (
          a.unit === 'UNIT VI' &&
          (
            a.name.trim().toLowerCase() === 'ilp' ||
            a.name.trim().toLowerCase().includes('integer linear programming')
          )
        ) {
          const response = await fetch(
            'http://127.0.0.1:5000/api/v1/algorithms/ilp'
          );

          const data = await response.json();

          if (!response.ok || data.success === false) {
            throw new Error(
              data.message || 'ILP API request failed'
            );
          }

          const decisionDetails = data.decision_variables
            .map(
              (item: {
                variable: string;
                location: string;
                value: number;
                meaning: string;
              }) =>
                `${item.variable} = ${item.value} | ${item.location} | ${item.meaning}`
            )
            .join('\n');

          const constraintDetails = data.constraints
            .map(
              (item: {
                constraint: string;
                expression: string;
                source: string;
                destination: string;
                left_value: number;
                right_value: number;
                satisfied: boolean;
              }) =>
                `${item.constraint}: ${item.expression} | ${item.source} - ${item.destination} | ${item.left_value} + ${item.right_value} >= 1 | ${
                  item.satisfied ? 'SATISFIED' : 'NOT SATISFIED'
                }`
            )
            .join('\n');

          const binaryDetails = data.binary_constraints.join('\n');

          const selectedLocations =
            data.selected_locations.length > 0
              ? data.selected_locations
                  .map((location: string) => `- ${location}`)
                  .join('\n')
              : 'No locations selected.';

          const optimalSolution = Object.entries(
            data.optimal_solution
          )
            .map(
              ([variable, value]) => `${variable} = ${value}`
            )
            .join('\n');

          setDemoOutput(
            `BACKEND RESULT\n\n` +
            `Algorithm: ${data.algorithm}\n\n` +
            `Method: ${data.method}\n\n` +
            `Number of Decision Variables: ${data.number_of_variables}\n\n` +
            `Number of Constraints: ${data.number_of_constraints}\n\n` +
            `Objective Type: ${data.objective_type}\n\n` +
            `Objective Function:\n${data.objective_function}\n\n` +
            `Decision Variables:\n${decisionDetails}\n\n` +
            `ILP Constraints:\n${constraintDetails}\n\n` +
            `Binary Constraints:\n${binaryDetails}\n\n` +
            `Optimal ILP Solution:\n${optimalSolution}\n\n` +
            `Minimum Objective Value: ${data.minimum_objective_value}\n\n` +
            `Calculated Objective Value: ${data.calculated_objective_value}\n\n` +
            `Selected Locations:\n${selectedLocations}\n\n` +
            `All Constraints Satisfied: ${
              data.all_constraints_satisfied ? 'YES' : 'NO'
            }\n\n` +
            `Objective Matches: ${data.objective_matches ? 'YES' : 'NO'}\n\n` +
            `Solution Verified: ${data.solution_verified ? 'YES' : 'NO'}\n\n` +
            `Result: ${data.result}\n\n` +
            `Time Complexity: ${data.time_complexity}\n\n` +
            `Space Complexity: ${data.space_complexity}`
          );

          return;
        }
// 4. Other algorithms - existing local demonstrations
await new Promise((resolve) => window.setTimeout(resolve, 300));

setDemoOutput(
  algorithmDemoOutputs[a.name] ??
    'Demo result ready for this algorithm.'
);

  } catch (error) {
    setDemoOutput(
      error instanceof Error
        ? `Unable to run algorithm: ${error.message}`
        : 'Unable to run algorithm. Please try again.'
    );
  } finally {
    setDemoRunning(false);
  }
};
  return (
    <>
      <SectionHeading
        eyebrow="DAA / ALGORITHM CATALOG"
        title="Algorithm Laboratory"
        description="Browse the complete Unit I–VI syllabus with front-end demonstrations, use cases and complexity."
        action={<MockLabel />}
      />

      <div className="workflow-strip">
        <span><MapPinned size={16} /> CITY GRAPH</span>
        <ArrowRight size={15} />
        <span><Binary size={16} /> ALGORITHM</span>
        <ArrowRight size={15} />
        <span><Gauge size={16} /> CALCULATION</span>
        <ArrowRight size={15} />
        <span><ShieldCheck size={16} /> DECISION</span>
      </div>

      <div className="algorithm-toolbar">
        <div className="search-box">
          <Search size={17} />
          <Input
            aria-label="Search algorithms"
            placeholder="Search algorithms..."
            value={query}
            onChange={(e) => setQuery(e.target.value)}
          />
        </div>

        <Select value={unit} onValueChange={setUnit}>
          <SelectTrigger className="unit-select">
            <SelectValue />
          </SelectTrigger>
          <SelectContent>
            {[
              'All units',
              'UNIT I',
              'UNIT II',
              'UNIT III',
              'UNIT IV',
              'UNIT V',
              'UNIT VI',
            ].map((u) => (
              <SelectItem key={u} value={u}>
                {u}
              </SelectItem>
            ))}
          </SelectContent>
        </Select>

        <span className="result-count">{shown.length} algorithms</span>
      </div>

      <div className="algorithm-groups">
        {['UNIT I', 'UNIT II', 'UNIT III', 'UNIT IV', 'UNIT V', 'UNIT VI'].map(
          (group) => {
            const list = shown.filter((a) => a.unit === group);

            if (!list.length) return null;

            return (
              <div className="algorithm-group" key={group}>
                <div className="group-heading">
                  <span>{group}</span>
                  <i />
                  {list.length} ALGORITHMS
                </div>

                <div className="algorithm-cards">
                  {list.map((a) => (
                    <div className="algorithm-card" key={a.name}>
                      <div className="algorithm-card-head">
                        <span className="algo-icon">
                          <Binary size={18} />
                        </span>
                        <Pill tone="success">READY</Pill>
                      </div>

                      <h3>{a.name}</h3>
                      <p>{a.problem}</p>

                      <div className="algo-use">
                        <span>APPLICATION</span>
                        <strong>{a.application}</strong>
                      </div>

                      <div className="algo-footer">
                        <span>TIME: {a.time}</span>
                        <Button
                          variant="link"
                          size="sm"
                          onClick={() => {
                            setSelected(a);
                            setDemoOutput('');
                          }}
                        >
                          View Algorithm <ArrowUpRight size={14} />
                        </Button>
                      </div>
                    </div>
                  ))}
                </div>
              </div>
            );
          }
        )}
      </div>

      <Dialog
        open={Boolean(selected)}
        onOpenChange={(v) => !v && setSelected(null)}
      >
        <DialogContent className="dialog-wide">
          <DialogHeader>
            <DialogTitle>{selected?.name}</DialogTitle>
            <DialogDescription>
              {selected?.unit} · Front-end algorithm laboratory
            </DialogDescription>
          </DialogHeader>

          {selected && (
            <div className="algorithm-detail">
              <div className="detail-pairs">
                <div>
                  <span>Problem solved</span>
                  <strong>{selected.problem}</strong>
                </div>
                <div>
                  <span>Application</span>
                  <strong>{selected.application}</strong>
                </div>
                <div>
                  <span>Time complexity</span>
                  <strong>{selected.time}</strong>
                </div>
                <div>
                  <span>Space complexity</span>
                  <strong>{selected.space}</strong>
                </div>
              </div>

              <Button
                onClick={() => runDemo(selected)}
                disabled={demoRunning}
              >
                <Play size={15} />
                {demoRunning ? 'Running demo…' : 'Run Demo'}
              </Button>

              {demoOutput && (
                <div className="result-banner" style={{ marginTop: 12 }}>
                  <div>
                    <span>DEMO RESULT</span>
                    <strong>{demoOutput}</strong>
                  </div>
                  <Pill tone="success">READY</Pill>
                </div>
              )}
            </div>
          )}
        </DialogContent>
      </Dialog>
    </>
  );
}
type SimulationRouteResult = {
  distance: number;
  estimated_time: number;
  path: string[];
  path_names: string[];
  reachable: boolean;
};

type SimulationApiResponse = {
  success: boolean;
  scenario_name: string;
  source: string;
  destination: string;
  blocked_roads: string[];
  baseline: SimulationRouteResult;
  simulated: SimulationRouteResult;
  impact: {
    distance_change: number;
    estimated_time_change: number;
    route_available: boolean;
  };
  context_metrics: {
    active_emergencies: number;
    available_shelter_capacity: number;
    people_affected: number;
  };
  note: string;
};
export function SimulationPage() {
  const [kind, setKind] = useState('Fire');
  const [location, setLocation] = useState('L10');
  const [severity, setSeverity] = useState('High');
  const [people, setPeople] = useState('150');
  const [run, setRun] = useState(false);
  const [step, setStep] = useState(0);
  const [destination, setDestination] = useState('L7');
const [blockedRoads, setBlockedRoads] = useState('R1,R3');

const [simulationResult, setSimulationResult] =
  useState<SimulationApiResponse | null>(null);

const [simulationLoading, setSimulationLoading] = useState(false);
const [simulationError, setSimulationError] = useState('');

  const selected = locations.find((l) => l.id === location)!;

  const team =
    initialTeams.find((t) => t.type === kind) ?? initialTeams[3];

  const nearest = shelters
    .map((s) => ({
      s,
      route: previewRoute(location, s.locationId, 'distance'),
    }))
    .filter((v) => v.route)
    .sort(
      (a, b) =>
        (a.route?.distance ?? Infinity) -
        (b.route?.distance ?? Infinity)
    )[0];

  const route = nearest?.route;

  const steps = [
    'Detect Emergency',
    'Analyze City Graph',
    'Find Optimal Route',
    'Assign Rescue Team',
    'Allocate Resources',
    'Calculate Evacuation Flow',
    'Recommend Shelter',
  ];

  const start = async () => {
  setRun(false);
  setStep(0);
  setSimulationError('');
  setSimulationResult(null);
  setSimulationLoading(true);

  const blockedRoadIds = blockedRoads
    .split(',')
    .map((road) => road.trim())
    .filter(Boolean);

  try {
    const response = await fetch(
      `${API_BASE_URL}/simulation/run`,
      {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          source: location,
          destination,
          blocked_roads: blockedRoadIds,
        }),
      }
    );

    if (!response.ok) {
      throw new Error(
        `Simulation API error: ${response.status}`
      );
    }

    const data =
      (await response.json()) as SimulationApiResponse;

    if (
      data.success !== true ||
      !data.baseline ||
      !data.simulated ||
      !data.impact
    ) {
      throw new Error('Invalid Simulation API response');
    }

    setSimulationResult(data);
    setRun(true);
    setStep(steps.length);
  } catch (error) {
    console.error('Simulation request failed:', error);

    setRun(false);
    setSimulationResult(null);

    setSimulationError(
      error instanceof Error
        ? error.message
        : 'Unable to run the simulation.'
    );
  } finally {
    setSimulationLoading(false);
  }
};

const reset = () => {
  setRun(false);
  setStep(0);
  setSimulationResult(null);
  setSimulationError('');
};
  const flow = Math.min(Number(people) || 0, 150);

  return (
    <>
      <SectionHeading
        eyebrow="OPERATIONS / SCENARIO PLANNING"
        title="Disaster Simulation"
        description="Configure a disaster scenario and walk through the complete decision pipeline."
        action={<MockLabel />}
      />

      <div className="evacuation-layout simulation-layout">
        <Panel title="Scenario Configuration">
          <div className="form-stack">
            <label>
              Disaster Type
              <Select
                value={kind}
                onValueChange={(v) => {
                  setKind(v);
                  reset();
                }}
              >
                <SelectTrigger>
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                  {['Fire', 'Flood', 'Accident'].map((v) => (
                    <SelectItem value={v} key={v}>
                      {v}
                    </SelectItem>
                  ))}
                </SelectContent>
              </Select>
            </label>

            <label>
              Emergency Location
              <Select
                value={location}
                onValueChange={(v) => {
                  setLocation(v);
                  reset();
                }}
              >
                <SelectTrigger>
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                {([
  ['L10', 'Industrial Area'],
  ['L12', 'Market Area'],
  ['L5', 'South Junction'],
] as const).map(([id, name]) => (
  <SelectItem value={id} key={id}>
    {name}
  </SelectItem>
))}
                </SelectContent>
              </Select>
            </label>

<label>
  Destination Shelter
  <Select
    value={destination}
    onValueChange={(value) => {
      setDestination(value);
      reset();
    }}
  >
    <SelectTrigger>
      <SelectValue />
    </SelectTrigger>
    <SelectContent>
      {shelters.map((shelter) => (
        <SelectItem key={shelter.id} value={shelter.locationId}>
          {shelter.name}
        </SelectItem>
      ))}
    </SelectContent>
  </Select>
</label>

<label>
  Roads to Block (comma-separated IDs)
  <Input
    value={blockedRoads}
    placeholder="R1,R3"
    onChange={(event) => {
      setBlockedRoads(event.target.value);
      reset();
    }}
  />
</label>
            <label>
              Severity
              <Select
                value={severity}
                onValueChange={(v) => {
                  setSeverity(v);
                  reset();
                }}
              >
                <SelectTrigger>
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                  {['Low', 'Medium', 'High', 'Critical'].map((v) => (
                    <SelectItem value={v} key={v}>
                      {v}
                    </SelectItem>
                  ))}
                </SelectContent>
              </Select>
            </label>

            <label>
              Affected People
              <Input
                type="number"
                min="1"
                value={people}
                onChange={(e) => {
                  setPeople(e.target.value);
                  reset();
                }}
              />
            </label>

            <div style={{ display: 'flex', gap: 8 }}>
              <Button
                className="w-full"
                onClick={start}
                disabled={!Number(people) || Number(people) < 1}
              >
                <Play size={16} /> Run Simulation
              </Button>

              <Button variant="outline" onClick={reset}>
                <RotateCcw size={16} />
              </Button>
            </div>
          </div>
        </Panel>

        <Panel title="Simulation Output" action={run ? <MockLabel /> : null}>
       {simulationError && (
  <p
    role="alert"
    style={{
      color: 'var(--destructive)',
      padding: '12px 16px',
    }}
  >
    {simulationError}
  </p>
)}
          {!run ? (
            <div className="result-empty">
              <Activity size={32} />
              <strong>Ready to simulate</strong>
              <p>Configure a disaster scenario and run the simulation.</p>
            </div>
          ) : (
            <>
              <div className="result-banner">
                <div>
                  <span>SCENARIO RESULT</span>
                  <strong>
                    {kind} at {selected.name}
                  </strong>
                </div>
                <Pill tone={severityTone(severity)}>
                  {severity.toUpperCase()}
                </Pill>
              </div>

              <div className="simulation-summary">
                <div>
                  <span>Affected area</span>
                  <strong>{selected.name}</strong>
                </div>
                <div>
                  <span>Affected people</span>
                  <strong>{Number(people).toLocaleString()}</strong>
                </div>
                <div>
                  <span>Rescue team</span>
               <strong>{team?.name ?? 'Unassigned'}</strong>
                </div>
                <div>
                  <span>Recommended shelter</span>
                 <strong>
  {shelters.find(
    (shelter) => shelter.locationId === destination
  )?.name ?? 'Unavailable'}
</strong>
                </div>
                <div>
                  <span>Required resource</span>
                  <strong>
                    {kind === 'Fire'
                      ? 'Water'
                      : kind === 'Flood'
                        ? 'Food'
                        : 'Medicine'}
                  </strong>
                </div>
                <div>
                  <span>Evacuation flow</span>
                  <strong>{flow} people / interval</strong>
                </div>
                <div>
                  <span>Optimal route</span>
                <strong>
  {simulationResult
    ? simulationResult.impact.route_available
      ? simulationResult.simulated.path_names.join(' → ')
      : 'No route available'
    : route?.names.join(' → ') ?? 'Unavailable'}
</strong>
                </div>
                <div>
                  <span>Algorithms</span>
                  <strong>Dijkstra · Scheduling · Matching · Max Flow</strong>
                </div>
                <div>
  <span>Baseline distance</span>
  <strong>
    {simulationResult ? `${simulationResult.baseline.distance} km` : '—'}
  </strong>
</div>

<div>
  <span>Simulated distance</span>
  <strong>
    {simulationResult ? `${simulationResult.simulated.distance} km` : '—'}
  </strong>
</div>

<div>
  <span>Distance change</span>
  <strong>
    {simulationResult
      ? `${simulationResult.impact.distance_change} km`
      : '—'}
  </strong>
</div>

<div>
  <span>Travel-time change</span>
  <strong>
    {simulationResult
      ? `${simulationResult.impact.estimated_time_change} min`
      : '—'}
  </strong>
</div>

<div>
  <span>Blocked roads</span>
  <strong>
    {simulationResult
      ? simulationResult.blocked_roads.join(', ') || 'None'
      : '—'}
  </strong>
</div>
              </div>
            </>
          )}
        </Panel>
      </div>

      {run && (
        <Panel title="Decision Pipeline" className="mt-panel">
          <div className="pipeline">
            {steps.map((s, i) => (
              <div className="pipeline-step" key={s}>
                <span>{String(i + 1).padStart(2, '0')}</span>
                {i < step ? (
                  <CheckCircle2 size={17} />
                ) : i === step ? (
                  <Activity size={17} />
                ) : (
                  <Clock3 size={17} />
                )}
                <strong>{s}</strong>
                <small>
                  {i < step
                    ? 'COMPLETED'
                    : i === step
                      ? 'RUNNING'
                      : 'QUEUED'}
                </small>
              </div>
            ))}
          </div>
        </Panel>
      )}
    </>
  );
}
type ReportsSummaryApiResponse = {
  success: boolean;
  data_source: string;
  report_type: string;
  emergencies: {
    active: number;
    total: number;
    people_affected: number;
    by_severity: Record<string, number>;
    by_type: Record<string, number>;
  };
  rescue_teams: {
    available: number;
    total: number;
  };
  shelters: {
    available: number;
    count: number;
    occupied: number;
    total_capacity: number;
  };
  resources: Record<string, { quantity: number; unit: string }>;
  roads: {
    total: number;
    by_status: Record<string, number>;
  };
};

export function ReportsPage() {
  const [reportData, setReportData] =
  useState<ReportsSummaryApiResponse | null>(null);

const [exportError, setExportError] = useState('');

useEffect(() => {
  let cancelled = false;

  async function loadReportSummary() {
    try {
      const response = await fetch(
        `${API_BASE_URL}/reports/summary`
      );

      if (!response.ok) {
        throw new Error(`Reports API error: ${response.status}`);
      }

      const data =
        (await response.json()) as ReportsSummaryApiResponse;

      if (
        data.success !== true ||
        !data.emergencies ||
        !data.rescue_teams ||
        !data.shelters
      ) {
        throw new Error('Invalid Reports summary response');
      }

      if (!cancelled) {
        setReportData(data);
      }
    } catch (error) {
      console.error('Failed to load Reports summary:', error);
    }
  }

  void loadReportSummary();

  return () => {
    cancelled = true;
  };
}, []);
  const monthData = [
    { name: 'Apr', count: 2 },
    { name: 'May', count: 3 },
    { name: 'Jun', count: 2 },
    { name: 'Jul', count: 4 },
    { name: 'Aug', count: 3 },
    { name: 'Sep', count: 3 },
  ];

  const executionData = [
    { name: 'BFS', ms: 8 },
    { name: 'DFS', ms: 7 },
    { name: 'Dijkstra', ms: 18 },
    { name: 'MST', ms: 15 },
    { name: 'Max Flow', ms: 29 },
  ];

  const total =
  reportData?.shelters.total_capacity ??
  shelters.reduce((a, s) => a + s.capacity, 0);

const occupied =
  reportData?.shelters.occupied ??
  shelters.reduce((a, s) => a + s.occupied, 0);

const activeIncidentCount =
  reportData?.emergencies.active ?? initialEmergencies.length;

const peopleAffected =
  reportData?.emergencies.people_affected ??
  initialEmergencies.reduce((sum, e) => sum + e.affected, 0);

const totalTeams =
  reportData?.rescue_teams.total ?? initialTeams.length;

const availableTeams =
  reportData?.rescue_teams.available ?? initialTeams.length;

const teamUtilization =
  totalTeams > 0
    ? ((totalTeams - availableTeams) / totalTeams) * 100
    : 0;

 const exportReport = async () => {
  setExportError('');

  try {
    const response = await fetch(
      `${API_BASE_URL}/reports/emergencies.csv`
    );

    if (!response.ok) {
      throw new Error(`CSV export failed: ${response.status}`);
    }

    const blob = await response.blob();
    const url = URL.createObjectURL(blob);

    const link = document.createElement('a');
    link.href = url;
    link.download = 'emergency_report.csv';

    document.body.appendChild(link);
    link.click();
    link.remove();

    window.setTimeout(() => URL.revokeObjectURL(url), 1000);
  } catch (error) {
    console.error('Report export failed:', error);

    setExportError(
      error instanceof Error
        ? error.message
        : 'Unable to export the report.'
    );
  }
};

   
  return (
    <>
      <SectionHeading
        eyebrow="INTELLIGENCE / ANALYTICS"
        title="Reports & Analytics"
        description="Operational trends, capacity and algorithm performance from the local project dataset."
        action={
          <Button onClick={exportReport}>
            <Download size={15} /> Export Report
          </Button>
        }
      />

      <div className="stats-grid">
        <Stat
          icon={AlertTriangle}
          label="Active Incidents"
          value={String(initialEmergencies.length)}
          note="Current snapshot"
          tone="critical"
        />
        <Stat
          icon={RouteIcon}
          label="Avg. Evacuation"
          value="18 min"
          note="Front-end estimate"
          tone="info"
        />
        <Stat
          icon={House}
          label="Shelter Occupancy"
          value={`${((occupied / total) * 100).toFixed(1)}%`}
          note={`${occupied} of ${total} spaces`}
          tone="success"
        />
        <Stat
          icon={ShieldCheck}
          label="Team Utilization"
          value="0%"
          note="No live dispatch yet"
          tone="info"
        />
      </div>

      <div className="report-grid">
        <Panel title="Emergencies Over Time">
          <div className="chart-wrap">
            <ResponsiveContainer width="100%" height="100%">
              <AreaChart
                data={monthData}
                margin={{ top: 15, right: 15, left: -30, bottom: 0 }}
              >
                <CartesianGrid
                  vertical={false}
                  stroke="var(--border)"
                  strokeDasharray="3 3"
                />
                <XAxis dataKey="name" tickLine={false} axisLine={false} />
                <YAxis tickLine={false} axisLine={false} />
                <Tooltip />
                <Area
                  type="monotone"
                  dataKey="count"
                  stroke="var(--primary)"
                  strokeWidth={2}
                  fill="var(--info-soft)"
                />
              </AreaChart>
            </ResponsiveContainer>
          </div>
        </Panel>

        <Panel title="Algorithm Execution Time">
          <div className="chart-wrap">
            <ResponsiveContainer width="100%" height="100%">
              <BarChart
                data={executionData}
                margin={{ top: 15, right: 15, left: -25, bottom: 0 }}
              >
                <CartesianGrid
                  vertical={false}
                  stroke="var(--border)"
                  strokeDasharray="3 3"
                />
                <XAxis dataKey="name" tickLine={false} axisLine={false} />
                <YAxis tickLine={false} axisLine={false} />
                <Tooltip />
                <Bar
                  dataKey="ms"
                  fill="var(--primary)"
                  radius={[3, 3, 0, 0]}
                />
              </BarChart>
            </ResponsiveContainer>
          </div>
        </Panel>

        <Panel title="Shelter Occupancy">
          <ShelterCards />
        </Panel>

        <Panel title="Resource Consumption">
          <div className="report-resource-list">
            {[
              { name: 'Water', used: 65 },
              { name: 'Food', used: 42 },
              { name: 'Medicine', used: 28 },
              { name: 'Blankets', used: 18 },
            ].map((r) => (
              <div key={r.name}>
                <div className="report-resource-label">
                  <strong>{r.name}</strong>
                  <span>{r.used}% used</span>
                </div>
                <CapacityBar
                  value={r.used}
                  tone={r.used > 60 ? 'warning' : 'success'}
                />
              </div>
            ))}
          </div>
        </Panel>

        <Panel title="Emergency History" className="report-wide">
          <EmergencyTable />
        </Panel>

        <Panel title="Rescue Team Utilization" className="report-wide">
          <DataTable
            headers={[
              'TEAM',
              'SPECIALTY',
              'LOCATION',
              'UTILIZATION',
              'STATUS',
            ]}
            rows={initialTeams.map((t) => [
              t.name,
              t.type,
              t.location,
              '0%',
              <Pill tone="success" key={t.id}>AVAILABLE</Pill>,
            ])}
          />
        </Panel>
      </div>
    </>
  );
}