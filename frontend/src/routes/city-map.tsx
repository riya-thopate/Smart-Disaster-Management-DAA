import { createFileRoute } from '@tanstack/react-router';
import { CityMapPage } from '@/pages/Views';
export const Route = createFileRoute('/city-map')({
 head: () => ({meta:[
  {title:'City Map & Network | SmartDisaster'},
  {name:'description',content:'City Map & Network in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:title',content:'City Map & Network | SmartDisaster'},
  {property:'og:description',content:'City Map & Network in SmartDisaster, a mock-data disaster response and DAA optimization platform.'},
  {property:'og:type',content:'website'},
  {name:'twitter:card',content:'summary_large_image'}
 ]}),
 component: CityMapPage
});
