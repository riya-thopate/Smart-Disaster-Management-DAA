import { createFileRoute } from '@tanstack/react-router';
import { Dashboard } from '@/pages/Views';
export const Route = createFileRoute('/')({
 head: () => ({meta:[
  {title:'Emergency Operations Dashboard | SmartDisaster'},
  {name:'description',content:'Emergency Operations Dashboard in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Emergency Operations Dashboard | SmartDisaster'},
  {property:'og:description',content:'Emergency Operations Dashboard in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: Dashboard
});
