import { createFileRoute } from '@tanstack/react-router';
import { SimulationPage } from '@/pages/Views';
export const Route = createFileRoute('/simulation')({
 head: () => ({meta:[
  {title:'Disaster Simulation | SmartDisaster'},
  {name:'description',content:'Disaster Simulation in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Disaster Simulation | SmartDisaster'},
  {property:'og:description',content:'Disaster Simulation in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: SimulationPage
});
