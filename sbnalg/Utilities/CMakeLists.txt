cet_make_library(INTERFACE
  SOURCE
    "AssnsCrosser.h"
    "PlaneCrossers.h"
  LIBRARIES INTERFACE
    larcorealg::Geometry
    larcorealg::CoreUtils
    canvas::canvas
  )

install_headers()
install_source()
install_fhicl()
