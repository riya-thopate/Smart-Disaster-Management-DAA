import { createFileRoute } from '@tanstack/react-router';
import { ResourcesPage } from '@/pages/Views';
export const Route = createFileRoute('/resources')({
 head: () => ({meta:[
  {title:'Resource Management | SmartDisaster'},
  {name:'description',content:'Resource Management in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Resource Management | SmartDisaster'},
  {property:'og:description',content:'Resource Management in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: ResourcesPage
});
