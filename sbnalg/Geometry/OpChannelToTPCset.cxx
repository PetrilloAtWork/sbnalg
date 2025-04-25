/**
 * @file   sbnalg/Geometry/OpChannelToTPCset.cxx
 * @brief  Provides the `sbn::OpChannelToTPCset` algorithm (implementation).
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   April 24, 2025
 * @see    sbnalg/Geometry/OpChannelToTPCset.h
 */

// library header
#include "sbnalg/Geometry/OpChannelToTPCset.h"

// LArSoft libraries
#include "larcorealg/Geometry/WireReadoutGeom.h"
#include "larcorealg/Geometry/GeometryCore.h"
#include "larcorealg/Geometry/CryostatGeo.h"
#include "larcorealg/Geometry/TPCGeo.h"
#include "larcorealg/Geometry/OpDetGeo.h"
#include "larcorealg/CoreUtils/counter.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h" // geo::Point_t

// C++ standard libraries
#include <limits>


// -----------------------------------------------------------------------------
namespace {
  
  /// Returns the TPC in `cryo` closest to `opDet`.
  geo::TPCGeo const& getClosestTPC
    (geo::OpDetGeo const& opDet, geo::CryostatGeo const& cryo)
  {
    geo::Point_t const& opDetPos = opDet.GetCenter();
    
    geo::TPCGeo const* closestTPC = nullptr;
    double closestTPCdistance2 = std::numeric_limits<double>::max();
    for (auto const& TPC: cryo.IterateTPCs()) {
      double const TPCdistance2 = (opDetPos - TPC.GetCenter()).Mag2();
      if (TPCdistance2 >= closestTPCdistance2) continue;
      closestTPC = &TPC;
      closestTPCdistance2 = TPCdistance2;
    } // for TPC
    
    return *closestTPC;
  } // getClosestTPC()
  
} // local namespace


// -----------------------------------------------------------------------------
std::vector<readout::TPCsetID> sbn::OpChannelToTPCset::buildOpDetToTPCsetMap
  (geo::GeometryCore const& geom, geo::WireReadoutGeom const& wireReadout)
{
  
  // despite the name, `MaxOpChannel()` returns the max channel plus one
  unsigned int const maxChannels = wireReadout.MaxOpChannel();
  
  std::vector<readout::TPCsetID> opDetToTPCsetMap
    (maxChannels, readout::TPCsetID{});
  
  for (unsigned int const channel: util::counter(maxChannels)) {
    
    geo::OpDetGeo const& opDet = wireReadout.OpDetGeoFromOpChannel(channel);
    
    geo::TPCGeo const& TPC = getClosestTPC(opDet, geom.Cryostat(opDet.ID()));
    
    opDetToTPCsetMap.at(channel) = wireReadout.TPCtoTPCset(TPC.ID());
    
  } // for optical detector channel
  
  return opDetToTPCsetMap;
}; // sbn::OpChannelToTPCset::buildOpDetToTPCsetMap()


// -----------------------------------------------------------------------------
