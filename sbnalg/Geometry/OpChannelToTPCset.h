/**
 * @file   sbnalg/Geometry/OpChannelToTPCset.h
 * @brief  Provides the `sbn::OpChannelToTPCset` algorithm.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   April 24, 2025
 * @see    sbnalg/Geometry/OpChannelToTPCset.cxx
 */

#ifndef SBNALG_GEOMETRY_OPCHANNELTOTPCSET_H
#define SBNALG_GEOMETRY_OPCHANNELTOTPCSET_H

// LArSoft libraries
#include "larcoreobj/SimpleTypesAndConstants/readout_types.h" // readout::TPCsetID

// C++ standard libraries
#include <vector>


// -----------------------------------------------------------------------------
// forward declarations
namespace geo {
  class GeometryCore;
  class WireReadoutGeom;
}

// -----------------------------------------------------------------------------
namespace sbn { class OpChannelToTPCset; }
/**
 * @brief A map from an optical detector channel to its TPC set.
 * 
 * This class creates and stores a map associating each optical detector channel
 * to a TPC set.
 * 
 * Optical detectors belong to a cryostat, but in SBN experiments they are
 * clearly associated to one physical TPC.
 * LArSoft geometry can split the physical TPC into logical ones, which are
 * represented with a `geo::TPCID` locator. This locator is not good enough for
 * the purpose of this algorithm, which intends to associate to the physical
 * TPC. The concept of TPC set, collection of all TPCs on the same drift volume,
 * is instead utilized as a proxy for the physical TPC.
 * 
 * This class is not lazy and creates the map on construction. Since map
 * creation uses a non-negligible amount of resources, the resulting object
 * is better cached rather than recreated on each use.
 * 
 * @note The interface mirrors the one of LArSoft geometry, and therefore uses
 *       plain integers as channel identifiers rather than `raw::Channel_t`.
 * 
 */
class sbn::OpChannelToTPCset {
  
  std::vector<readout::TPCsetID> fOpDetToTPCsetMap; ///< The map.
  
    public:
  
  /// Constructor: creates the map acquires the geometry provider.
  OpChannelToTPCset
    (geo::GeometryCore const& geom, geo::WireReadoutGeom const& wireReadout);
  
  // --- BEGIN ---  Access interface  ------------------------------------------
  /// @name Access interface
  /// @{
  
  /// Returns the number of channels for which we store an associated TPC.
  int maxChannels() const;
  
  /// Returns whether the specified optical detector `channel` is in the map.
  bool hasChannel(int channel) const;
  
  /**
   * @brief Returns the TPC set ID associated to `channel`.
   * @param channel the optical detector channel to be queried
   * @return the TPC set ID associated to `channel`, invalid if none
   */
  readout::TPCsetID TPCsetOf(int channel) const;
  
  /// Alias of `TPCsetOf()`.
  /// @see `TPCsetOf()`
  readout::TPCsetID operator[](int channel) const;
  
  /// @}
  // ---  END  ---  Access interface  ------------------------------------------
  
  
  /**
   * @brief Builds and returns the map from a optical detector channel to TPC.
   * @param geom geometry service provider
   * @param wireReadout readout mapping service provider
   * @return the newly built map as an array with channel as index.
   * 
   * The returned map is a C++ vector with entries enough to cover the whole
   * optical detector channel range, and the index of the vector is the optical
   * detector channel number.
   * Channels associated to no TPC are mapped to an invalid TPC set ID.
   * 
   */
  static std::vector<readout::TPCsetID> buildOpDetToTPCsetMap
    (geo::GeometryCore const& geom, geo::WireReadoutGeom const& wireReadout);
  
  
}; // sbn::CachedPointTracker


// -----------------------------------------------------------------------------
// ---  Inline implementation
// -----------------------------------------------------------------------------
inline sbn::OpChannelToTPCset::OpChannelToTPCset
  (geo::GeometryCore const& geom, geo::WireReadoutGeom const& wireReadout)
  : fOpDetToTPCsetMap{ buildOpDetToTPCsetMap(geom, wireReadout) }
{
}


// -----------------------------------------------------------------------------
inline int sbn::OpChannelToTPCset::maxChannels() const {
  return static_cast<int>(fOpDetToTPCsetMap.size());
}


// -----------------------------------------------------------------------------
inline bool sbn::OpChannelToTPCset::hasChannel(int channel) const {
  return channel < maxChannels();
}


// -----------------------------------------------------------------------------
inline readout::TPCsetID sbn::OpChannelToTPCset::TPCsetOf(int channel) const {
  return hasChannel(channel)? fOpDetToTPCsetMap[channel]: readout::TPCsetID{};
}


// -----------------------------------------------------------------------------
inline readout::TPCsetID sbn::OpChannelToTPCset::operator[](int channel) const {
  return TPCsetOf(channel);
}


// -----------------------------------------------------------------------------


#endif // SBNALG_GEOMETRY_OPCHANNELTOTPCSET_H
