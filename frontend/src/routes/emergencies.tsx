import { createFileRoute } from '@tanstack/react-router';
import { EmergenciesPage } from '@/pages/Views';
export const Route = createFileRoute('/emergencies')({
 head: () => ({meta:[
  {title:'Emergency Management | SmartDisaster'},
  {name:'description',content:'Emergency Management in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Emergency Management | SmartDisaster'},
  {property:'og:description',content:'Emergency Management in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: EmergenciesPage
});
