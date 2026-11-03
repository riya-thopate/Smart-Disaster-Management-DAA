import { createFileRoute } from '@tanstack/react-router';
import { ReportsPage } from '@/pages/Views';
export const Route = createFileRoute('/reports')({
 head: () => ({meta:[
  {title:'Reports & Analytics | SmartDisaster'},
  {name:'description',content:'Reports & Analytics in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Reports & Analytics | SmartDisaster'},
  {property:'og:description',content:'Reports & Analytics in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: ReportsPage
});
