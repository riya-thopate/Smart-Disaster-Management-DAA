import type { ReactNode } from 'react';
import { cn } from '@/lib/utils';

export function Panel({title,action,children,className}: {title?:string;action?:ReactNode;children:ReactNode;className?:string}){return <section className={cn('panel',className)}>{title&&<div className="panel-head"><h2>{title}</h2>{action}</div>}{children}</section>}
export function Pill({children,tone='neutral'}:{children:ReactNode;tone?:'critical'|'warning'|'success'|'info'|'neutral'}){return <span className={`pill pill-${tone}`}><span className="pill-dot"/>{children}</span>}
export function SectionHeading({eyebrow,title,description,action}:{eyebrow?:string;title:string;description?:string;action?:ReactNode}){return <div className="section-heading"><div>{eyebrow&&<div className="eyebrow">{eyebrow}</div>}<h1>{title}</h1>{description&&<p>{description}</p>}</div>{action}</div>}
export function Stat({icon:Icon,label,value,note,tone='info'}:{icon:React.ComponentType<{size?:number}>;label:string;value:string;note:string;tone?:'critical'|'warning'|'success'|'info'}){return <div className="stat"><div className="stat-top"><span>{label}</span><span className={`stat-icon tone-${tone}`}><Icon size={19}/></span></div><strong>{value}</strong><div className={`stat-note tone-text-${tone}`}><span className="tiny-dot"/>{note}</div></div>}
export function DataTable({headers,rows}:{headers:string[];rows:ReactNode[][]}){return <div className="table-scroll"><table className="data-table"><thead><tr>{headers.map(h=><th key={h}>{h}</th>)}</tr></thead><tbody>{rows.map((row,i)=><tr key={i}>{row.map((cell,j)=><td key={j}>{cell}</td>)}</tr>)}</tbody></table></div>}
export function CapacityBar({value,tone='success'}:{value:number;tone?:'success'|'warning'|'critical'}){return <div className="capacity-track"><div className={`capacity-fill fill-${tone}`} style={{width:`${Math.min(100,value)}%`}}/></div>}
export function MockLabel(){return <span className="mock-label">SIMULATED DATA</span>}
