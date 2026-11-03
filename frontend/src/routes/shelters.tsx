import { createFileRoute } from '@tanstack/react-router';
import { SheltersPage } from '@/pages/Views';
export const Route = createFileRoute('/shelters')({
 head: () => ({meta:[
  {title:'Shelter Management | SmartDisaster'},
  {name:'description',content:'Shelter Management in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Shelter Management | SmartDisaster'},
  {property:'og:description',content:'Shelter Management in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: SheltersPage
});
