import { createFileRoute } from '@tanstack/react-router';
import { EvacuationPage } from '@/pages/Views';
export const Route = createFileRoute('/evacuation')({
 head: () => ({meta:[
  {title:'Evacuation Route Optimization | SmartDisaster'},
  {name:'description',content:'Evacuation Route Optimization in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Evacuation Route Optimization | SmartDisaster'},
  {property:'og:description',content:'Evacuation Route Optimization in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: EvacuationPage
});
