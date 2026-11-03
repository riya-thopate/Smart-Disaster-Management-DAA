import { createFileRoute } from '@tanstack/react-router';
import { TeamsPage } from '@/pages/Views';
export const Route = createFileRoute('/rescue-teams')({
 head: () => ({meta:[
  {title:'Rescue Teams | SmartDisaster'},
  {name:'description',content:'Rescue Teams in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Rescue Teams | SmartDisaster'},
  {property:'og:description',content:'Rescue Teams in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: TeamsPage
});
