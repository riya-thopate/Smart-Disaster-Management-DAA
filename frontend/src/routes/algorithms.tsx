import { createFileRoute } from '@tanstack/react-router';
import { AlgorithmsPage } from '@/pages/Views';
export const Route = createFileRoute('/algorithms')({
 head: () => ({meta:[
  {title:'Algorithm Laboratory | SmartDisaster'},
  {name:'description',content:'Algorithm Laboratory in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'Algorithm Laboratory | SmartDisaster'},
  {property:'og:description',content:'Algorithm Laboratory in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: AlgorithmsPage
});
