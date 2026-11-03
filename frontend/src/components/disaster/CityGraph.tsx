import { useState } from 'react';
import { locations, roads, teams, emergencies, shelters, type Location } from '@/data/mock';
import { Pill } from './kit';
import { MapPin, Navigation, Shield, Building2, House, AlertTriangle } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { cn } from '@/lib/utils';

const icons = {Junction:Navigation,Hospital:Building2,Shelter:House,'Emergency Center':Shield,'Emergency Area':AlertTriangle};
export function CityGraph({compact=false,highlight=[]}:{compact?:boolean;highlight?:string[]}){
 const [selected,setSelected]=useState<Location|null>(null);
 const [filter,setFilter]=useState('All');
 const filtered=locations.filter(l=>filter==='All'||filter==='Emergency Areas'&&l.type==='Emergency Area'||filter==='Junctions'&&l.type==='Junction'||filter==='Hospitals'&&l.type==='Hospital'||filter==='Shelters'&&l.type==='Shelter'||filter==='Emergency Centers'&&l.type==='Emergency Center');
 const filters=['All','Junctions','Hospitals','Shelters','Emergency Centers','Emergency Areas'];
 return <div className={cn('graph-section',compact&&'graph-compact')}>
  {!compact&&<div className="graph-filters">{filters.map(f=><Button key={f} variant={filter===f?'default':'outline'} size="sm" onClick={()=>setFilter(f)}>{f}</Button>)}</div>}
  <div className="graph-layout"><div className="graph-canvas">
   <div className="graph-grid"/>
   <svg viewBox="0 0 1000 600" preserveAspectRatio="none" className="roads-svg" aria-hidden="true">{roads.map((r,i)=>{
    const a=locations.find(l=>l.id===r.from),b=locations.find(l=>l.id===r.to); if(!a||!b)return null;
    const isHighlighted=highlight.includes(a.id)&&highlight.includes(b.id)&&Math.abs(highlight.indexOf(a.id)-highlight.indexOf(b.id))===1;
    return <g key={i}><line x1={a.x*10} y1={a.y*6} x2={b.x*10} y2={b.y*6} className={isHighlighted?'road-line road-highlight':'road-line'}/>{!compact&&<text x={(a.x+b.x)*5} y={(a.y+b.y)*3-5} textAnchor="middle" className="road-label">{r.distance} km</text>}</g>})}</svg>
   {locations.map(l=>{const Icon=icons[l.type];const emergency=emergencies.some(e=>e.locationId===l.id);return <Button key={l.id} variant="ghost" size="icon" title={`${l.id} · ${l.name}`} aria-label={`${l.id} ${l.name}`} onClick={()=>setSelected(l)} className={cn('map-node',`node-${emergency?'danger':l.type.toLowerCase().replaceAll(' ','-')}`,selected?.id===l.id&&'node-selected',!compact&&!filtered.some(f=>f.id===l.id)&&'node-dim',highlight.includes(l.id)&&'node-route')} style={{left:`${l.x}%`,top:`${l.y}%`}}><Icon size={compact?14:18}/><span className="node-caption">{l.name}</span></Button>})}
   <div className="graph-scale">CITY NETWORK <span>12 NODES · 15 ROADS</span></div>
  </div>{!compact&&<aside className="graph-detail">{selected?<><div className="eyebrow">LOCATION DETAILS</div><h3>{selected.name}</h3><p className="detail-id">{selected.id} · {selected.type}</p><div className="detail-row"><span>Connected roads</span><strong>{roads.filter(r=>r.from===selected.id||r.to===selected.id).length}</strong></div><div className="detail-row"><span>Emergency status</span><Pill tone={emergencies.some(e=>e.locationId===selected.id)?'critical':'success'}>{emergencies.some(e=>e.locationId===selected.id)?'Active':'Clear'}</Pill></div><div className="detail-row"><span>Nearby shelters</span><strong>{shelters.filter(s=>s.locationId===selected.id).map(s=>s.name).join(', ')||'See network'}</strong></div><div className="detail-row"><span>Rescue teams</span><strong>{teams.filter(t=>t.locationId===selected.id).map(t=>t.name).join(', ')||'None stationed'}</strong></div><h4>Connected roads</h4>{roads.filter(r=>r.from===selected.id||r.to===selected.id).map(r=><div className="road-detail" key={`${r.from}-${r.to}`}><span>{selected.id} → {r.from===selected.id?r.to:r.from}</span><strong>{r.distance} km · {r.time} min</strong><small>Capacity {r.capacity} · {r.status}</small></div>)}</>:<div className="detail-empty"><MapPin size={30}/><h3>Select a location</h3><p>Choose a node on the city network to inspect its roads and response status.</p></div>}</aside>}</div>
  <div className="graph-legend"><span><i className="legend-dot legend-critical"/>Emergency</span><span><i className="legend-dot legend-green"/>Shelter</span><span><i className="legend-dot legend-blue"/>Hospital / Center</span><span><i className="legend-dot legend-gray"/>Junction</span></div>
 </div>
}
