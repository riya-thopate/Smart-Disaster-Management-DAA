import { useState } from 'react';
import { Link, useRouterState } from '@tanstack/react-router';
import { LayoutDashboard, Map, Siren, Route, ShieldCheck, House, Package, Binary, Play, ChartNoAxesCombined, Bell, Menu, X, Radio, ChevronDown, Activity } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { cn } from '@/lib/utils';
import type { ReactNode } from 'react';
const nav = [
 {to:'/',label:'Dashboard',icon:LayoutDashboard},{to:'/city-map',label:'City Map',icon:Map},{to:'/emergencies',label:'Emergencies',icon:Siren},{to:'/evacuation',label:'Evacuation',icon:Route},{to:'/rescue-teams',label:'Rescue Teams',icon:ShieldCheck},{to:'/shelters',label:'Shelters',icon:House},{to:'/resources',label:'Resources',icon:Package},{to:'/algorithms',label:'Algorithms',icon:Binary},{to:'/simulation',label:'Simulation',icon:Play},{to:'/reports',label:'Reports',icon:ChartNoAxesCombined}
] as const;
export function Shell({children}:{children:ReactNode}){
 const [open,setOpen]=useState(false),[notice,setNotice]=useState(false);
 const path=useRouterState({select:s=>s.location.pathname});
 const current=nav.find(n=>n.to===path)?.label??'Dashboard';
 return <div className="app-shell"><aside className={cn('sidebar',open&&'sidebar-open')}>
  <div className="brand"><div className="brand-mark"><Activity size={22}/></div><div><strong>SmartDisaster</strong><span>DAA Optimization System</span></div><Button className="sidebar-close" variant="ghost" size="icon" aria-label="Close menu" onClick={()=>setOpen(false)}><X size={18}/></Button></div>
  <div className="nav-label">WORKSPACE</div><nav className="sidebar-nav">{nav.map(item=>{const Icon=item.icon;return <Link to={item.to} key={item.to} className={cn('nav-item',path===item.to&&'nav-active')} onClick={()=>setOpen(false)}><Icon size={18}/><span>{item.label}</span>{item.label==='Emergencies'&&<small>3</small>}</Link>})}</nav>
  <div className="sidebar-bottom"><div className="sidebar-status"><span className="status-pulse"/><div><strong>System Status</strong><span>All Systems Operational</span></div></div><div className="sidebar-foot">SMARTDISASTER <span>V 1.0 / DEMO</span></div></div>
 </aside>{open&&<div className="mobile-backdrop" onClick={()=>setOpen(false)}/>}
 <div className="main-wrap"><header className="topbar"><div className="topbar-left"><Button variant="ghost" size="icon" className="menu-toggle" aria-label="Open menu" onClick={()=>setOpen(true)}><Menu size={20}/></Button><span className="breadcrumb">Operations <span>/</span> <strong>{current}</strong></span></div><div className="topbar-right"><span className="live-status"><Radio size={14}/> SYSTEM ONLINE</span><div className="header-divider"/><div className="notification-wrap"><Button variant="ghost" size="icon" aria-label="Notifications" title="Notifications" className="notification-button" onClick={()=>setNotice(!notice)}><Bell size={19}/><i/></Button>{notice&&<div className="notification-pop"><strong>Notifications</strong><p>3 active incidents require monitoring.</p><Link to="/emergencies" onClick={()=>setNotice(false)}>View emergencies →</Link></div>}</div><div className="header-divider"/><div className="profile"><span className="avatar">OP</span><span className="profile-text"><strong>Operator</strong><small>Administrator</small></span><ChevronDown size={14}/></div></div></header><main className="page-content">{children}</main><footer className="main-footer"><span>SmartDisaster · DAA Optimization System</span><span>Simulation environment · Mock data</span></footer></div></div>
}
