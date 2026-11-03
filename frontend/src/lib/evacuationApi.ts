export const API_BASE_URL = 'http://127.0.0.1:5000/api/v1';

export interface EvacuationRouteResponse {
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
}

function isValid(d: unknown): d is EvacuationRouteResponse {
  const r = d as EvacuationRouteResponse;
  return !!r && r.success === true && typeof r.algorithm === 'string' && typeof r.distance === 'number' &&
    typeof r.estimated_time === 'number' && Array.isArray(r.path) && Array.isArray(r.path_names) &&
    r.path.length > 0 && r.path.length === r.path_names.length;
}

export async function fetchEvacuationRoute(source: string, destination: string): Promise<EvacuationRouteResponse> {
  if (!source) throw new Error('Please select an emergency location.');
  if (!destination) throw new Error('Please select a destination shelter.');
  if (source === destination) throw new Error('Source and destination must be different.');
  let res: Response;
  try {
    res = await fetch(`${API_BASE_URL}/evacuation/route`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ source, destination }),
    });
  } catch {
    throw new Error('Backend unavailable. Make sure the Flask server is running at 127.0.0.1:5000.');
  }
  let data: unknown;
  try { data = await res.json(); } catch { throw new Error('Invalid backend response (not JSON).'); }
  if (!res.ok) {
    const msg = (data as { error?: string; message?: string })?.error ?? (data as { message?: string })?.message;
    throw new Error(msg || `Backend error (${res.status}).`);
  }
  if (!isValid(data)) throw new Error('Invalid backend response format.');
  return data;
}
