cet_test(AssnsCrosser_test
  LIBRARIES
    sbnalg::Utilities
    sbnalg::Test
    canvas::canvas
    cetlib::cetlib
  USE_BOOST_UNIT
  )

cet_test(PlaneCrossers_test USE_BOOST_UNIT
  SOURCE PlaneCrossers_test.cxx
  LIBRARIES PRIVATE
    larcoreobj::headers
    larcorealg::Geometry
)

install_source()
