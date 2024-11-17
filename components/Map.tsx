"use client";
import React, { useState, useEffect } from "react";
import { Loader } from "@googlemaps/js-api-loader";


export function Map(){
const mapRef = React.useRef<HTMLDivElement>(null);
useEffect(() => {
  const initMap = async () => {
    const loader = new Loader({
      apiKey: "AIzaSyDDGQQEA0grrP2FPL_iNZhEX0hlxTOMuwM",
      version: "weekly"
      });
      const {Map} = await loader.importLibrary("maps");
      const position = {
        lat: 40.7128,
        lng: -74.0059
        };
        const mapOptions: google.maps.MapOptions = {
          center: position,
          zoom: 16
          };
          // setup the map
          const map = new Map(mapRef.current as HTMLDivElement, mapOptions);
        }
        initMap();
      });
      return (<div style = {{height: '600px'}} ref={mapRef}> </div>);
}
export default Map;