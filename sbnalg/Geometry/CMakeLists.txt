cet_make_library(
  SOURCE
    "CachedPointTracker.cxx"
    "OpChannelToTPCset.cxx"
  LIBRARIES
    larcorealg::Geometry
    larcoreobj::headers
  )

install_headers()
install_source()
