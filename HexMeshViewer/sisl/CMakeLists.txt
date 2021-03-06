###############################################################################
# MODULE       : CMakeList.txt
# DESCRIPTION  : Configuration file for cmake
# GENERATED BY : jbt
#         WITH : mmxmake 
###############################################################################
# This software falls under the GNU general public license and
# comes WITHOUT ANY WARRANTY WHATSOEVER. See the file LICENSE for details.
# If you don't have this file, write to the Free Software Foundation, Inc.,
# 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
###############################################################################


cmake_minimum_required(VERSION 2.6)

PROJECT(sisl)

set(PROJECT_VERSION 0.1)
set(FIND_NAME Sisl)

string (TOUPPER ${PROJECT_NAME} PKG_NAME)
string (TOLOWER ${PROJECT_NAME} LIB_NAME)

set(${PKG_NAME}_PACKAGE ${PROJECT_NAME}        )
set(${PKG_NAME}_PREFIX  ${CMAKE_INSTALL_PREFIX})

set(CPACK_PACKAGE_NAME    ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_SUMMARY ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT "mathemagix-devel@mathemagix.org")
set(CPACK_GENERATOR "TGZ;STGZ;RPM")
set(CPACK_SOURCE_GENERATOR "TGZ")


include (CPack)
enable_testing()

set(PKG_HTML_DIR  "html/${PROJECT_NAME}")
set(PKG_DOX_DIR   "dox/${PROJECT_NAME}")

IF (USE_${PKG_NAME})
  set(PKG_SUFFIX _${PROJECT_NAME} )
  set(PKG_PREFIX  ${PROJECT_NAME}_)
ELSE (USE_${PKG_NAME})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR}/CMakeModules/)
  include(CheckTypeSize)
  check_type_size("long long" LONG_LONG_INT BUILTIN_TYPES_ONLY)
  link_directories(${CMAKE_BINARY_DIR}/lib)
ENDIF (USE_${PKG_NAME})

IF (HAVE_LONG_LONG_INT)
  set(${PKG_NAME}_HAVE_LONG_LONG_INT 1)
ENDIF (HAVE_LONG_LONG_INT)

set(SEARCH_DIR ${PROJECT_SOURCE_DIR}/..)
set(${PKG_NAME}_VERSION 0.1)
message("== Configuring sisl 0.1")
IF (NOT USE_${PKG_NAME})
   option(LIBS "enable/disable library build" ON )
   option(TEST "enable/disable test build" OFF )
   option(APP "enable/disable application build" ON )
   option(GLUE "enable/disable glue with mmx interpreter" ON )
   option(AXL "enable/disable axel plugins" OFF )
   option(DOC "enable/disable automatic documentation" OFF )
   option(BENCH "enable/disable benchmarks" OFF )
   option(EXPL "enable/disable examples" OFF )
   option(EMBEDDED "enable/disable static inclusion" OFF )
   option(SHARED "enable/disable shared library build" ON )
   option(STATIC "enable/disable static library build" OFF )
   option(DEBUG "enable/disable debugging" OFF )
   option(OPTIMIZE "enable/disable optimization" ON )
   option(EXCEPTIONS "enable/disable cpp exceptions" ON )
   option(THREADS "enable/disable threads" OFF )
   option(EXTERNAL "enable/disable external packages" OFF )
   option(DEV "enable/disable development mode" OFF )
ENDIF (NOT USE_${PKG_NAME})


IF(NOT LIBS)
  set(APP OFF)
  set(TEST OFF)
  set(GLUE OFF)
  set(STATIC OFF)
  set(SHARED OFF)
ENDIF(NOT LIBS)

IF(EMBEDDED)
  set(STATIC ON)
ENDIF(EMBEDDED)

IF(DEBUG)
  set(${PKG_NAME}_CXX_FLAGS "${${PKG_NAME}_CXX_FLAGS} -g -O2")
ENDIF(DEBUG) 

IF(OPTIMIZE)
  set(${PKG_NAME}_CXX_FLAGS  "${${PKG_NAME}_CXX_FLAGS} -ansi -Wall -O3 -fexpensive-optimizations")
ENDIF(OPTIMIZE)

set(${PKG_NAME}_CXX_FLAGS  "${${PKG_NAME}_CXX_FLAGS} -ftemplate-depth-100")

IF(NOT USE_${PKG_NAME})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${${PKG_NAME}_CXX_FLAGS}")
ENDIF(NOT USE_${PKG_NAME})
IF (NOT USE_${PKG_NAME})

######################################################################
#  LIBS
######################################################################
IF (LIBS)
   message(STATUS  "[*] enable library build (LIBS=ON)")
   set(ENABLE_LIBS ON)
ELSE(LIBS)
   message(STATUS  "[ ] disable library build (LIBS=OFF)")
ENDIF (LIBS)

######################################################################
#  TEST
######################################################################
IF (TEST)
   message(STATUS  "[*] enable test build (TEST=ON)")
   set(ENABLE_TEST ON)
ELSE(TEST)
   message(STATUS  "[ ] disable test build (TEST=OFF)")
ENDIF (TEST)

######################################################################
#  APP
######################################################################
IF (APP)
   message(STATUS  "[*] enable application build (APP=ON)")
   set(ENABLE_APP ON)
ELSE(APP)
   message(STATUS  "[ ] disable application build (APP=OFF)")
ENDIF (APP)

######################################################################
#  GLUE
######################################################################
IF (GLUE)
   message(STATUS  "[*] enable glue with mmx interpreter (GLUE=ON)")
   set(ENABLE_GLUE ON)
ELSE(GLUE)
   message(STATUS  "[ ] disable glue with mmx interpreter (GLUE=OFF)")
ENDIF (GLUE)

######################################################################
#  AXL
######################################################################
IF (AXL)
   message(STATUS  "[*] enable axel plugins (AXL=ON)")
   set(ENABLE_AXL ON)
ELSE(AXL)
   message(STATUS  "[ ] disable axel plugins (AXL=OFF)")
ENDIF (AXL)

######################################################################
#  DOC
######################################################################
IF (DOC)
   message(STATUS  "[*] enable automatic documentation (DOC=ON)")
   set(ENABLE_DOC ON)
ELSE(DOC)
   message(STATUS  "[ ] disable automatic documentation (DOC=OFF)")
ENDIF (DOC)

######################################################################
#  BENCH
######################################################################
IF (BENCH)
   message(STATUS  "[*] enable benchmarks (BENCH=ON)")
   set(ENABLE_BENCH ON)
ELSE(BENCH)
   message(STATUS  "[ ] disable benchmarks (BENCH=OFF)")
ENDIF (BENCH)

######################################################################
#  EXPL
######################################################################
IF (EXPL)
   message(STATUS  "[*] enable examples (EXPL=ON)")
   set(ENABLE_EXPL ON)
ELSE(EXPL)
   message(STATUS  "[ ] disable examples (EXPL=OFF)")
ENDIF (EXPL)

######################################################################
#  EMBEDDED
######################################################################
IF (EMBEDDED)
   message(STATUS  "[*] enable static inclusion (EMBEDDED=ON)")
   set(ENABLE_EMBEDDED ON)
ELSE(EMBEDDED)
   message(STATUS  "[ ] disable static inclusion (EMBEDDED=OFF)")
ENDIF (EMBEDDED)

######################################################################
#  SHARED
######################################################################
IF (SHARED)
   message(STATUS  "[*] enable shared library build (SHARED=ON)")
   set(ENABLE_SHARED ON)
ELSE(SHARED)
   message(STATUS  "[ ] disable shared library build (SHARED=OFF)")
ENDIF (SHARED)

######################################################################
#  STATIC
######################################################################
IF (STATIC)
   message(STATUS  "[*] enable static library build (STATIC=ON)")
   set(ENABLE_STATIC ON)
ELSE(STATIC)
   message(STATUS  "[ ] disable static library build (STATIC=OFF)")
ENDIF (STATIC)

######################################################################
#  DEBUG
######################################################################
IF (DEBUG)
   message(STATUS  "[*] enable debugging (DEBUG=ON)")
   set(ENABLE_DEBUG ON)
ELSE(DEBUG)
   message(STATUS  "[ ] disable debugging (DEBUG=OFF)")
ENDIF (DEBUG)

######################################################################
#  OPTIMIZE
######################################################################
IF (OPTIMIZE)
   message(STATUS  "[*] enable optimization (OPTIMIZE=ON)")
   set(ENABLE_OPTIMIZE ON)
ELSE(OPTIMIZE)
   message(STATUS  "[ ] disable optimization (OPTIMIZE=OFF)")
ENDIF (OPTIMIZE)

######################################################################
#  EXCEPTIONS
######################################################################
IF (EXCEPTIONS)
   message(STATUS  "[*] enable cpp exceptions (EXCEPTIONS=ON)")
   set(ENABLE_EXCEPTIONS ON)
ELSE(EXCEPTIONS)
   message(STATUS  "[ ] disable cpp exceptions (EXCEPTIONS=OFF)")
ENDIF (EXCEPTIONS)

######################################################################
#  THREADS
######################################################################
IF (THREADS)
   message(STATUS  "[*] enable threads (THREADS=ON)")
   set(ENABLE_THREADS ON)
ELSE(THREADS)
   message(STATUS  "[ ] disable threads (THREADS=OFF)")
ENDIF (THREADS)

######################################################################
#  EXTERNAL
######################################################################
IF (EXTERNAL)
   message(STATUS  "[*] enable external packages (EXTERNAL=ON)")
   set(ENABLE_EXTERNAL ON)
ELSE(EXTERNAL)
   message(STATUS  "[ ] disable external packages (EXTERNAL=OFF)")
ENDIF (EXTERNAL)

######################################################################
#  DEV
######################################################################
IF (DEV)
   message(STATUS  "[*] enable development mode (DEV=ON)")
   set(ENABLE_DEV ON)
ELSE(DEV)
   message(STATUS  "[ ] disable development mode (DEV=OFF)")
ENDIF (DEV)

######################################################################
#  SIMD 
######################################################################
IF(SIMD)
  add_definitions(-m${SIMD})
  message(STATUS  "[*] simd level (SIMD=${SIMD})")
ELSE(SIMD)
  message(STATUS  "[ ] simd level (SIMD=OFF)")
ENDIF(SIMD)
ENDIF (NOT USE_${PKG_NAME})
IF(LIBS)
ENDIF(LIBS)

configure_file( "${PROJECT_SOURCE_DIR}/CMakeModules/Find${FIND_NAME}.cmake.in"
                "${CMAKE_BINARY_DIR}/CMakeModules/Find${FIND_NAME}.cmake"
                @ONLY IMMEDIATE)

configure_file( "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}-config.hpp.in"
		"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}-config.hpp" )
add_definitions(-DHAVE_CONFIG_H)


IF(NOT USE_${PKG_NAME})
   set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
   set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
   set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
   include_directories( ${CMAKE_BINARY_DIR}/include )
ENDIF(NOT USE_${PKG_NAME})



######################################################################
# Library
######################################################################
include_directories( ${PROJECT_SOURCE_DIR}/include
                     ${PROJECT_SOURCE_DIR}/src
                     ${PROJECT_SOURCE_DIR}/axl )
IF(LIBS)
               
install( DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/ 
	 DESTINATION include/${PROJECT_NAME}
         FILES_MATCHING PATTERN "*.hpp" PATTERN ".svn" EXCLUDE )	


set (PKG_SRC 
  ${PROJECT_SOURCE_DIR}/src/GoReadWrite.cpp
  ${PROJECT_SOURCE_DIR}/src/construct.c
  ${PROJECT_SOURCE_DIR}/src/crvarctang.c
  ${PROJECT_SOURCE_DIR}/src/crvcrvtang.c
  ${PROJECT_SOURCE_DIR}/src/crvlintang.c
  ${PROJECT_SOURCE_DIR}/src/destruct.c
  ${PROJECT_SOURCE_DIR}/src/ev_cv_off.c
  ${PROJECT_SOURCE_DIR}/src/eval_2_crv.c
  ${PROJECT_SOURCE_DIR}/src/evalcrvarc.c
  ${PROJECT_SOURCE_DIR}/src/hp_s1880.c
  ${PROJECT_SOURCE_DIR}/src/intjoinper.c
  ${PROJECT_SOURCE_DIR}/src/make3D.c
  ${PROJECT_SOURCE_DIR}/src/makecvkreg.c
  ${PROJECT_SOURCE_DIR}/src/makesfkreg.c
  ${PROJECT_SOURCE_DIR}/src/maketracks.c
  ${PROJECT_SOURCE_DIR}/src/mk_cv_cycl.c
  ${PROJECT_SOURCE_DIR}/src/newknots.c
  ${PROJECT_SOURCE_DIR}/src/pickcrvsf.c
  ${PROJECT_SOURCE_DIR}/src/pocrvtang.c
  ${PROJECT_SOURCE_DIR}/src/refine_all.c
  ${PROJECT_SOURCE_DIR}/src/s1001.c
  ${PROJECT_SOURCE_DIR}/src/s1011.c
  ${PROJECT_SOURCE_DIR}/src/s1012.c
  ${PROJECT_SOURCE_DIR}/src/s1013.c
  ${PROJECT_SOURCE_DIR}/src/s1014.c
  ${PROJECT_SOURCE_DIR}/src/s1015.c
  ${PROJECT_SOURCE_DIR}/src/s1016.c
  ${PROJECT_SOURCE_DIR}/src/s1017.c
  ${PROJECT_SOURCE_DIR}/src/s1018.c
  ${PROJECT_SOURCE_DIR}/src/s1021.c
  ${PROJECT_SOURCE_DIR}/src/s1022.c
  ${PROJECT_SOURCE_DIR}/src/s1023.c
  ${PROJECT_SOURCE_DIR}/src/s1024.c
  ${PROJECT_SOURCE_DIR}/src/s1025.c
  ${PROJECT_SOURCE_DIR}/src/s1119.c
  ${PROJECT_SOURCE_DIR}/src/s1161.c
  ${PROJECT_SOURCE_DIR}/src/s1162.c
  ${PROJECT_SOURCE_DIR}/src/s1172.c
  ${PROJECT_SOURCE_DIR}/src/s1173.c
  ${PROJECT_SOURCE_DIR}/src/s1174.c
  ${PROJECT_SOURCE_DIR}/src/s1190.c
  ${PROJECT_SOURCE_DIR}/src/s1192.c
  ${PROJECT_SOURCE_DIR}/src/s1219.c
  ${PROJECT_SOURCE_DIR}/src/s1220.c
  ${PROJECT_SOURCE_DIR}/src/s1221.c
  ${PROJECT_SOURCE_DIR}/src/s1222.c
  ${PROJECT_SOURCE_DIR}/src/s1223.c
  ${PROJECT_SOURCE_DIR}/src/s1224.c
  ${PROJECT_SOURCE_DIR}/src/s1225.c
  ${PROJECT_SOURCE_DIR}/src/s1226.c
  ${PROJECT_SOURCE_DIR}/src/s1227.c
  ${PROJECT_SOURCE_DIR}/src/s1231.c
  ${PROJECT_SOURCE_DIR}/src/s1232.c
  ${PROJECT_SOURCE_DIR}/src/s1233.c
  ${PROJECT_SOURCE_DIR}/src/s1235.c
  ${PROJECT_SOURCE_DIR}/src/s1236.c
  ${PROJECT_SOURCE_DIR}/src/s1237.c
  ${PROJECT_SOURCE_DIR}/src/s1238.c
  ${PROJECT_SOURCE_DIR}/src/s1239.c
  ${PROJECT_SOURCE_DIR}/src/s1240.c
  ${PROJECT_SOURCE_DIR}/src/s1241.c
  ${PROJECT_SOURCE_DIR}/src/s1243.c
  ${PROJECT_SOURCE_DIR}/src/s1244.c
  ${PROJECT_SOURCE_DIR}/src/s1245.c
  ${PROJECT_SOURCE_DIR}/src/s1251.c
  ${PROJECT_SOURCE_DIR}/src/s1252.c
  ${PROJECT_SOURCE_DIR}/src/s1301.c
  ${PROJECT_SOURCE_DIR}/src/s1302.c
  ${PROJECT_SOURCE_DIR}/src/s1303.c
  ${PROJECT_SOURCE_DIR}/src/s1304.c
  ${PROJECT_SOURCE_DIR}/src/s1305.c
  ${PROJECT_SOURCE_DIR}/src/s1306.c
  ${PROJECT_SOURCE_DIR}/src/s1307.c
  ${PROJECT_SOURCE_DIR}/src/s1308.c
  ${PROJECT_SOURCE_DIR}/src/s1309.c
  ${PROJECT_SOURCE_DIR}/src/s1310.c
  ${PROJECT_SOURCE_DIR}/src/s1311.c
  ${PROJECT_SOURCE_DIR}/src/s1312.c
  ${PROJECT_SOURCE_DIR}/src/s1313.c
  ${PROJECT_SOURCE_DIR}/src/s1314.c
  ${PROJECT_SOURCE_DIR}/src/s1315.c
  ${PROJECT_SOURCE_DIR}/src/s1316.c
  ${PROJECT_SOURCE_DIR}/src/s1317.c
  ${PROJECT_SOURCE_DIR}/src/s1318.c
  ${PROJECT_SOURCE_DIR}/src/s1319.c
  ${PROJECT_SOURCE_DIR}/src/s1320.c
  ${PROJECT_SOURCE_DIR}/src/s1321.c
  ${PROJECT_SOURCE_DIR}/src/s1322.c
  ${PROJECT_SOURCE_DIR}/src/s1323.c
  ${PROJECT_SOURCE_DIR}/src/s1324.c
  ${PROJECT_SOURCE_DIR}/src/s1325.c
  ${PROJECT_SOURCE_DIR}/src/s1327.c
  ${PROJECT_SOURCE_DIR}/src/s1328.c
  ${PROJECT_SOURCE_DIR}/src/s1329.c
  ${PROJECT_SOURCE_DIR}/src/s1330.c
  ${PROJECT_SOURCE_DIR}/src/s1331.c
  ${PROJECT_SOURCE_DIR}/src/s1332.c
  ${PROJECT_SOURCE_DIR}/src/s1333.c
  ${PROJECT_SOURCE_DIR}/src/s1333count.c
  ${PROJECT_SOURCE_DIR}/src/s1333cycli.c
  ${PROJECT_SOURCE_DIR}/src/s1334.c
  ${PROJECT_SOURCE_DIR}/src/s1340.c
  ${PROJECT_SOURCE_DIR}/src/s1341.c
  ${PROJECT_SOURCE_DIR}/src/s1342.c
  ${PROJECT_SOURCE_DIR}/src/s1343.c
  ${PROJECT_SOURCE_DIR}/src/s1345.c
  ${PROJECT_SOURCE_DIR}/src/s1346.c
  ${PROJECT_SOURCE_DIR}/src/s1347.c
  ${PROJECT_SOURCE_DIR}/src/s1348.c
  ${PROJECT_SOURCE_DIR}/src/s1349.c
  ${PROJECT_SOURCE_DIR}/src/s1350.c
  ${PROJECT_SOURCE_DIR}/src/s1351.c
  ${PROJECT_SOURCE_DIR}/src/s1352.c
  ${PROJECT_SOURCE_DIR}/src/s1353.c
  ${PROJECT_SOURCE_DIR}/src/s1354.c
  ${PROJECT_SOURCE_DIR}/src/s1355.c
  ${PROJECT_SOURCE_DIR}/src/s1356.c
  ${PROJECT_SOURCE_DIR}/src/s1357.c
  ${PROJECT_SOURCE_DIR}/src/s1358.c
  ${PROJECT_SOURCE_DIR}/src/s1359.c
  ${PROJECT_SOURCE_DIR}/src/s1360.c
  ${PROJECT_SOURCE_DIR}/src/s1361.c
  ${PROJECT_SOURCE_DIR}/src/s1362.c
  ${PROJECT_SOURCE_DIR}/src/s1363.c
  ${PROJECT_SOURCE_DIR}/src/s1364.c
  ${PROJECT_SOURCE_DIR}/src/s1365.c
  ${PROJECT_SOURCE_DIR}/src/s1366.c
  ${PROJECT_SOURCE_DIR}/src/s1367.c
  ${PROJECT_SOURCE_DIR}/src/s1369.c
  ${PROJECT_SOURCE_DIR}/src/s1370.c
  ${PROJECT_SOURCE_DIR}/src/s1371.c
  ${PROJECT_SOURCE_DIR}/src/s1372.c
  ${PROJECT_SOURCE_DIR}/src/s1373.c
  ${PROJECT_SOURCE_DIR}/src/s1374.c
  ${PROJECT_SOURCE_DIR}/src/s1375.c
  ${PROJECT_SOURCE_DIR}/src/s1376.c
  ${PROJECT_SOURCE_DIR}/src/s1377.c
  ${PROJECT_SOURCE_DIR}/src/s1378.c
  ${PROJECT_SOURCE_DIR}/src/s1379.c
  ${PROJECT_SOURCE_DIR}/src/s1380.c
  ${PROJECT_SOURCE_DIR}/src/s1381.c
  ${PROJECT_SOURCE_DIR}/src/s1382.c
  ${PROJECT_SOURCE_DIR}/src/s1383.c
  ${PROJECT_SOURCE_DIR}/src/s1384.c
  ${PROJECT_SOURCE_DIR}/src/s1385.c
  ${PROJECT_SOURCE_DIR}/src/s1386.c
  ${PROJECT_SOURCE_DIR}/src/s1387.c
  ${PROJECT_SOURCE_DIR}/src/s1388.c
  ${PROJECT_SOURCE_DIR}/src/s1389.c
  ${PROJECT_SOURCE_DIR}/src/s1390.c
  ${PROJECT_SOURCE_DIR}/src/s1391.c
  ${PROJECT_SOURCE_DIR}/src/s1393.c
  ${PROJECT_SOURCE_DIR}/src/s1399.c
  ${PROJECT_SOURCE_DIR}/src/s1401.c
  ${PROJECT_SOURCE_DIR}/src/s1421.c
  ${PROJECT_SOURCE_DIR}/src/s1422.c
  ${PROJECT_SOURCE_DIR}/src/s1424.c
  ${PROJECT_SOURCE_DIR}/src/s1425.c
  ${PROJECT_SOURCE_DIR}/src/s1435.c
  ${PROJECT_SOURCE_DIR}/src/s1436.c
  ${PROJECT_SOURCE_DIR}/src/s1437.c
  ${PROJECT_SOURCE_DIR}/src/s1438.c
  ${PROJECT_SOURCE_DIR}/src/s1439.c
  ${PROJECT_SOURCE_DIR}/src/s1440.c
  ${PROJECT_SOURCE_DIR}/src/s1450.c
  ${PROJECT_SOURCE_DIR}/src/s1451.c
  ${PROJECT_SOURCE_DIR}/src/s1452.c
  ${PROJECT_SOURCE_DIR}/src/s1500.c
  ${PROJECT_SOURCE_DIR}/src/s1501.c
  ${PROJECT_SOURCE_DIR}/src/s1502.c
  ${PROJECT_SOURCE_DIR}/src/s1503.c
  ${PROJECT_SOURCE_DIR}/src/s1504.c
  ${PROJECT_SOURCE_DIR}/src/s1505.c
  ${PROJECT_SOURCE_DIR}/src/s1506.c
  ${PROJECT_SOURCE_DIR}/src/s1507.c
  ${PROJECT_SOURCE_DIR}/src/s1508.c
  ${PROJECT_SOURCE_DIR}/src/s1510.c
  ${PROJECT_SOURCE_DIR}/src/s1511.c
  ${PROJECT_SOURCE_DIR}/src/s1512.c
  ${PROJECT_SOURCE_DIR}/src/s1513.c
  ${PROJECT_SOURCE_DIR}/src/s1514.c
  ${PROJECT_SOURCE_DIR}/src/s1515.c
  ${PROJECT_SOURCE_DIR}/src/s1516.c
  ${PROJECT_SOURCE_DIR}/src/s1517.c
  ${PROJECT_SOURCE_DIR}/src/s1518.c
  ${PROJECT_SOURCE_DIR}/src/s1520.c
  ${PROJECT_SOURCE_DIR}/src/s1521.c
  ${PROJECT_SOURCE_DIR}/src/s1522.c
  ${PROJECT_SOURCE_DIR}/src/s1528.c
  ${PROJECT_SOURCE_DIR}/src/s1529.c
  ${PROJECT_SOURCE_DIR}/src/s1530.c
  ${PROJECT_SOURCE_DIR}/src/s1531.c
  ${PROJECT_SOURCE_DIR}/src/s1534.c
  ${PROJECT_SOURCE_DIR}/src/s1535.c
  ${PROJECT_SOURCE_DIR}/src/s1536.c
  ${PROJECT_SOURCE_DIR}/src/s1537.c
  ${PROJECT_SOURCE_DIR}/src/s1538.c
  ${PROJECT_SOURCE_DIR}/src/s1539.c
  ${PROJECT_SOURCE_DIR}/src/s1540.c
  ${PROJECT_SOURCE_DIR}/src/s1541.c
  ${PROJECT_SOURCE_DIR}/src/s1542.c
  ${PROJECT_SOURCE_DIR}/src/s1600.c
  ${PROJECT_SOURCE_DIR}/src/s1601.c
  ${PROJECT_SOURCE_DIR}/src/s1602.c
  ${PROJECT_SOURCE_DIR}/src/s1603.c
  ${PROJECT_SOURCE_DIR}/src/s1604.c
  ${PROJECT_SOURCE_DIR}/src/s1605.c
  ${PROJECT_SOURCE_DIR}/src/s1606.c
  ${PROJECT_SOURCE_DIR}/src/s1607.c
  ${PROJECT_SOURCE_DIR}/src/s1608.c
  ${PROJECT_SOURCE_DIR}/src/s1609.c
  ${PROJECT_SOURCE_DIR}/src/s1611.c
  ${PROJECT_SOURCE_DIR}/src/s1612.c
  ${PROJECT_SOURCE_DIR}/src/s1613.c
  ${PROJECT_SOURCE_DIR}/src/s1613bez.c
  ${PROJECT_SOURCE_DIR}/src/s1614.c
  ${PROJECT_SOURCE_DIR}/src/s1615.c
  ${PROJECT_SOURCE_DIR}/src/s1616.c
  ${PROJECT_SOURCE_DIR}/src/s1617.c
  ${PROJECT_SOURCE_DIR}/src/s1618.c
  ${PROJECT_SOURCE_DIR}/src/s1619.c
  ${PROJECT_SOURCE_DIR}/src/s1620.c
  ${PROJECT_SOURCE_DIR}/src/s1630.c
  ${PROJECT_SOURCE_DIR}/src/s1631.c
  ${PROJECT_SOURCE_DIR}/src/s1700.c
  ${PROJECT_SOURCE_DIR}/src/s1701.c
  ${PROJECT_SOURCE_DIR}/src/s1705.c
  ${PROJECT_SOURCE_DIR}/src/s1706.c
  ${PROJECT_SOURCE_DIR}/src/s1707.c
  ${PROJECT_SOURCE_DIR}/src/s1708.c
  ${PROJECT_SOURCE_DIR}/src/s1710.c
  ${PROJECT_SOURCE_DIR}/src/s1711.c
  ${PROJECT_SOURCE_DIR}/src/s1712.c
  ${PROJECT_SOURCE_DIR}/src/s1713.c
  ${PROJECT_SOURCE_DIR}/src/s1714.c
  ${PROJECT_SOURCE_DIR}/src/s1715.c
  ${PROJECT_SOURCE_DIR}/src/s1716.c
  ${PROJECT_SOURCE_DIR}/src/s1720.c
  ${PROJECT_SOURCE_DIR}/src/s1730.c
  ${PROJECT_SOURCE_DIR}/src/s1731.c
  ${PROJECT_SOURCE_DIR}/src/s1732.c
  ${PROJECT_SOURCE_DIR}/src/s1733.c
  ${PROJECT_SOURCE_DIR}/src/s1741.c
  ${PROJECT_SOURCE_DIR}/src/s1750.c
  ${PROJECT_SOURCE_DIR}/src/s1753.c
  ${PROJECT_SOURCE_DIR}/src/s1754.c
  ${PROJECT_SOURCE_DIR}/src/s1755.c
  ${PROJECT_SOURCE_DIR}/src/s1770.c
  ${PROJECT_SOURCE_DIR}/src/s17702d.c
  ${PROJECT_SOURCE_DIR}/src/s1771.c
  ${PROJECT_SOURCE_DIR}/src/s1772.c
  ${PROJECT_SOURCE_DIR}/src/s1773.c
  ${PROJECT_SOURCE_DIR}/src/s1774.c
  ${PROJECT_SOURCE_DIR}/src/s1775.c
  ${PROJECT_SOURCE_DIR}/src/s1780.c
  ${PROJECT_SOURCE_DIR}/src/s1785.c
  ${PROJECT_SOURCE_DIR}/src/s1786.c
  ${PROJECT_SOURCE_DIR}/src/s1787.c
  ${PROJECT_SOURCE_DIR}/src/s1788.c
  ${PROJECT_SOURCE_DIR}/src/s1789.c
  ${PROJECT_SOURCE_DIR}/src/s1790.c
  ${PROJECT_SOURCE_DIR}/src/s1791.c
  ${PROJECT_SOURCE_DIR}/src/s1792.c
  ${PROJECT_SOURCE_DIR}/src/s1795.c
  ${PROJECT_SOURCE_DIR}/src/s1796.c
  ${PROJECT_SOURCE_DIR}/src/s1797.c
  ${PROJECT_SOURCE_DIR}/src/s1830.c
  ${PROJECT_SOURCE_DIR}/src/s1834.c
  ${PROJECT_SOURCE_DIR}/src/s1839.c
  ${PROJECT_SOURCE_DIR}/src/s1840.c
  ${PROJECT_SOURCE_DIR}/src/s1850.c
  ${PROJECT_SOURCE_DIR}/src/s1851.c
  ${PROJECT_SOURCE_DIR}/src/s1852.c
  ${PROJECT_SOURCE_DIR}/src/s1853.c
  ${PROJECT_SOURCE_DIR}/src/s1854.c
  ${PROJECT_SOURCE_DIR}/src/s1855.c
  ${PROJECT_SOURCE_DIR}/src/s1856.c
  ${PROJECT_SOURCE_DIR}/src/s1857.c
  ${PROJECT_SOURCE_DIR}/src/s1858.c
  ${PROJECT_SOURCE_DIR}/src/s1859.c
  ${PROJECT_SOURCE_DIR}/src/s1860.c
  ${PROJECT_SOURCE_DIR}/src/s1870.c
  ${PROJECT_SOURCE_DIR}/src/s1871.c
  ${PROJECT_SOURCE_DIR}/src/s1880.c
  ${PROJECT_SOURCE_DIR}/src/s1890.c
  ${PROJECT_SOURCE_DIR}/src/s1891.c
  ${PROJECT_SOURCE_DIR}/src/s1893.c
  ${PROJECT_SOURCE_DIR}/src/s1894.c
  ${PROJECT_SOURCE_DIR}/src/s1896.c
  ${PROJECT_SOURCE_DIR}/src/s1897.c
  ${PROJECT_SOURCE_DIR}/src/s1900.c
  ${PROJECT_SOURCE_DIR}/src/s1901.c
  ${PROJECT_SOURCE_DIR}/src/s1902.c
  ${PROJECT_SOURCE_DIR}/src/s1903.c
  ${PROJECT_SOURCE_DIR}/src/s1904.c
  ${PROJECT_SOURCE_DIR}/src/s1905.c
  ${PROJECT_SOURCE_DIR}/src/s1906.c
  ${PROJECT_SOURCE_DIR}/src/s1907.c
  ${PROJECT_SOURCE_DIR}/src/s1908.c
  ${PROJECT_SOURCE_DIR}/src/s1909.c
  ${PROJECT_SOURCE_DIR}/src/s1910.c
  ${PROJECT_SOURCE_DIR}/src/s1911.c
  ${PROJECT_SOURCE_DIR}/src/s1912.c
  ${PROJECT_SOURCE_DIR}/src/s1916.c
  ${PROJECT_SOURCE_DIR}/src/s1917.c
  ${PROJECT_SOURCE_DIR}/src/s1918.c
  ${PROJECT_SOURCE_DIR}/src/s1919.c
  ${PROJECT_SOURCE_DIR}/src/s1920.c
  ${PROJECT_SOURCE_DIR}/src/s1921.c
  ${PROJECT_SOURCE_DIR}/src/s1924.c
  ${PROJECT_SOURCE_DIR}/src/s1925.c
  ${PROJECT_SOURCE_DIR}/src/s1926.c
  ${PROJECT_SOURCE_DIR}/src/s1927.c
  ${PROJECT_SOURCE_DIR}/src/s1930.c
  ${PROJECT_SOURCE_DIR}/src/s1931.c
  ${PROJECT_SOURCE_DIR}/src/s1931unit.c
  ${PROJECT_SOURCE_DIR}/src/s1932.c
  ${PROJECT_SOURCE_DIR}/src/s1933.c
  ${PROJECT_SOURCE_DIR}/src/s1934.c
  ${PROJECT_SOURCE_DIR}/src/s1935.c
  ${PROJECT_SOURCE_DIR}/src/s1936.c
  ${PROJECT_SOURCE_DIR}/src/s1937.c
  ${PROJECT_SOURCE_DIR}/src/s1938.c
  ${PROJECT_SOURCE_DIR}/src/s1940.c
  ${PROJECT_SOURCE_DIR}/src/s1941.c
  ${PROJECT_SOURCE_DIR}/src/s1942.c
  ${PROJECT_SOURCE_DIR}/src/s1943.c
  ${PROJECT_SOURCE_DIR}/src/s1944.c
  ${PROJECT_SOURCE_DIR}/src/s1945.c
  ${PROJECT_SOURCE_DIR}/src/s1946.c
  ${PROJECT_SOURCE_DIR}/src/s1947.c
  ${PROJECT_SOURCE_DIR}/src/s1948.c
  ${PROJECT_SOURCE_DIR}/src/s1949.c
  ${PROJECT_SOURCE_DIR}/src/s1950.c
  ${PROJECT_SOURCE_DIR}/src/s1951.c
  ${PROJECT_SOURCE_DIR}/src/s1953.c
  ${PROJECT_SOURCE_DIR}/src/s1954.c
  ${PROJECT_SOURCE_DIR}/src/s1955.c
  ${PROJECT_SOURCE_DIR}/src/s1956.c
  ${PROJECT_SOURCE_DIR}/src/s1957.c
  ${PROJECT_SOURCE_DIR}/src/s1958.c
  ${PROJECT_SOURCE_DIR}/src/s1959.c
  ${PROJECT_SOURCE_DIR}/src/s1960.c
  ${PROJECT_SOURCE_DIR}/src/s1961.c
  ${PROJECT_SOURCE_DIR}/src/s1962.c
  ${PROJECT_SOURCE_DIR}/src/s1963.c
  ${PROJECT_SOURCE_DIR}/src/s1965.c
  ${PROJECT_SOURCE_DIR}/src/s1966.c
  ${PROJECT_SOURCE_DIR}/src/s1967.c
  ${PROJECT_SOURCE_DIR}/src/s1968.c
  ${PROJECT_SOURCE_DIR}/src/s1986.c
  ${PROJECT_SOURCE_DIR}/src/s1987.c
  ${PROJECT_SOURCE_DIR}/src/s1988.c
  ${PROJECT_SOURCE_DIR}/src/s1989.c
  ${PROJECT_SOURCE_DIR}/src/s1990.c
  ${PROJECT_SOURCE_DIR}/src/s1991.c
  ${PROJECT_SOURCE_DIR}/src/s1992.c
  ${PROJECT_SOURCE_DIR}/src/s1993.c
  ${PROJECT_SOURCE_DIR}/src/s1994.c
  ${PROJECT_SOURCE_DIR}/src/s2500.c
  ${PROJECT_SOURCE_DIR}/src/s2501.c
  ${PROJECT_SOURCE_DIR}/src/s2502.c
  ${PROJECT_SOURCE_DIR}/src/s2503.c
  ${PROJECT_SOURCE_DIR}/src/s2504.c
  ${PROJECT_SOURCE_DIR}/src/s2505.c
  ${PROJECT_SOURCE_DIR}/src/s2506.c
  ${PROJECT_SOURCE_DIR}/src/s2507.c
  ${PROJECT_SOURCE_DIR}/src/s2508.c
  ${PROJECT_SOURCE_DIR}/src/s2509.c
  ${PROJECT_SOURCE_DIR}/src/s2510.c
  ${PROJECT_SOURCE_DIR}/src/s2511.c
  ${PROJECT_SOURCE_DIR}/src/s2512.c
  ${PROJECT_SOURCE_DIR}/src/s2513.c
  ${PROJECT_SOURCE_DIR}/src/s2514.c
  ${PROJECT_SOURCE_DIR}/src/s2515.c
  ${PROJECT_SOURCE_DIR}/src/s2516.c
  ${PROJECT_SOURCE_DIR}/src/s2532.c
  ${PROJECT_SOURCE_DIR}/src/s2533.c
  ${PROJECT_SOURCE_DIR}/src/s2534.c
  ${PROJECT_SOURCE_DIR}/src/s2535.c
  ${PROJECT_SOURCE_DIR}/src/s2536.c
  ${PROJECT_SOURCE_DIR}/src/s2540.c
  ${PROJECT_SOURCE_DIR}/src/s2541.c
  ${PROJECT_SOURCE_DIR}/src/s2542.c
  ${PROJECT_SOURCE_DIR}/src/s2543.c
  ${PROJECT_SOURCE_DIR}/src/s2544.c
  ${PROJECT_SOURCE_DIR}/src/s2545.c
  ${PROJECT_SOURCE_DIR}/src/s2550.c
  ${PROJECT_SOURCE_DIR}/src/s2551.c
  ${PROJECT_SOURCE_DIR}/src/s2553.c
  ${PROJECT_SOURCE_DIR}/src/s2554.c
  ${PROJECT_SOURCE_DIR}/src/s2555.c
  ${PROJECT_SOURCE_DIR}/src/s2556.c
  ${PROJECT_SOURCE_DIR}/src/s2557.c
  ${PROJECT_SOURCE_DIR}/src/s2558.c
  ${PROJECT_SOURCE_DIR}/src/s2559.c
  ${PROJECT_SOURCE_DIR}/src/s2560.c
  ${PROJECT_SOURCE_DIR}/src/s2561.c
  ${PROJECT_SOURCE_DIR}/src/s2562.c
  ${PROJECT_SOURCE_DIR}/src/s6addcurve.c
  ${PROJECT_SOURCE_DIR}/src/s6affdist.c
  ${PROJECT_SOURCE_DIR}/src/s6ang.c
  ${PROJECT_SOURCE_DIR}/src/s6angle.c
  ${PROJECT_SOURCE_DIR}/src/s6castelja.c
  ${PROJECT_SOURCE_DIR}/src/s6chpar.c
  ${PROJECT_SOURCE_DIR}/src/s6crss.c
  ${PROJECT_SOURCE_DIR}/src/s6crvature.c
  ${PROJECT_SOURCE_DIR}/src/s6crvcheck.c
  ${PROJECT_SOURCE_DIR}/src/s6curvrad.c
  ${PROJECT_SOURCE_DIR}/src/s6decomp.c
  ${PROJECT_SOURCE_DIR}/src/s6degnorm.c
  ${PROJECT_SOURCE_DIR}/src/s6dertopt.c
  ${PROJECT_SOURCE_DIR}/src/s6diff.c
  ${PROJECT_SOURCE_DIR}/src/s6dist.c
  ${PROJECT_SOURCE_DIR}/src/s6dline.c
  ${PROJECT_SOURCE_DIR}/src/s6dplane.c
  ${PROJECT_SOURCE_DIR}/src/s6drawseq.c
  ${PROJECT_SOURCE_DIR}/src/s6equal.c
  ${PROJECT_SOURCE_DIR}/src/s6err.c
  ${PROJECT_SOURCE_DIR}/src/s6existbox.c
  ${PROJECT_SOURCE_DIR}/src/s6findfac.c
  ${PROJECT_SOURCE_DIR}/src/s6fndintv.c
  ${PROJECT_SOURCE_DIR}/src/s6herm.c
  ${PROJECT_SOURCE_DIR}/src/s6herm_bez.c
  ${PROJECT_SOURCE_DIR}/src/s6idcon.c
  ${PROJECT_SOURCE_DIR}/src/s6idcpt.c
  ${PROJECT_SOURCE_DIR}/src/s6idedg.c
  ${PROJECT_SOURCE_DIR}/src/s6identify.c
  ${PROJECT_SOURCE_DIR}/src/s6idget.c
  ${PROJECT_SOURCE_DIR}/src/s6idint.c
  ${PROJECT_SOURCE_DIR}/src/s6idklist.c
  ${PROJECT_SOURCE_DIR}/src/s6idkpt.c
  ${PROJECT_SOURCE_DIR}/src/s6idlis.c
  ${PROJECT_SOURCE_DIR}/src/s6idnpt.c
  ${PROJECT_SOURCE_DIR}/src/s6idput.c
  ${PROJECT_SOURCE_DIR}/src/s6inv4.c
  ${PROJECT_SOURCE_DIR}/src/s6invert.c
  ${PROJECT_SOURCE_DIR}/src/s6knotmult.c
  ${PROJECT_SOURCE_DIR}/src/s6length.c
  ${PROJECT_SOURCE_DIR}/src/s6line.c
  ${PROJECT_SOURCE_DIR}/src/s6lprj.c
  ${PROJECT_SOURCE_DIR}/src/s6lufacp.c
  ${PROJECT_SOURCE_DIR}/src/s6lusolp.c
  ${PROJECT_SOURCE_DIR}/src/s6metric.c
  ${PROJECT_SOURCE_DIR}/src/s6move.c
  ${PROJECT_SOURCE_DIR}/src/s6mulvec.c
  ${PROJECT_SOURCE_DIR}/src/s6mvec.c
  ${PROJECT_SOURCE_DIR}/src/s6newbox.c
  ${PROJECT_SOURCE_DIR}/src/s6norm.c
  ${PROJECT_SOURCE_DIR}/src/s6ratder.c
  ${PROJECT_SOURCE_DIR}/src/s6rotax.c
  ${PROJECT_SOURCE_DIR}/src/s6rotmat.c
  ${PROJECT_SOURCE_DIR}/src/s6schoen.c
  ${PROJECT_SOURCE_DIR}/src/s6scpr.c
  ${PROJECT_SOURCE_DIR}/src/s6sortpar.c
  ${PROJECT_SOURCE_DIR}/src/s6sratder.c
  ${PROJECT_SOURCE_DIR}/src/s6strider.c
  ${PROJECT_SOURCE_DIR}/src/s6takunion.c
  ${PROJECT_SOURCE_DIR}/src/s6twonorm.c
  ${PROJECT_SOURCE_DIR}/src/s9adsimp.c
  ${PROJECT_SOURCE_DIR}/src/s9adstep.c
  ${PROJECT_SOURCE_DIR}/src/s9boundimp.c
  ${PROJECT_SOURCE_DIR}/src/s9boundit.c
  ${PROJECT_SOURCE_DIR}/src/s9clipimp.c
  ${PROJECT_SOURCE_DIR}/src/s9clipit.c
  ${PROJECT_SOURCE_DIR}/src/s9conmarch.c
  ${PROJECT_SOURCE_DIR}/src/s9iterate.c
  ${PROJECT_SOURCE_DIR}/src/s9iterimp.c
  ${PROJECT_SOURCE_DIR}/src/s9smplknot.c
  ${PROJECT_SOURCE_DIR}/src/s9surmarch.c
  ${PROJECT_SOURCE_DIR}/src/sh1260.c
  ${PROJECT_SOURCE_DIR}/src/sh1261.c
  ${PROJECT_SOURCE_DIR}/src/sh1262.c
  ${PROJECT_SOURCE_DIR}/src/sh1263.c
  ${PROJECT_SOURCE_DIR}/src/sh1365.c
  ${PROJECT_SOURCE_DIR}/src/sh1369.c
  ${PROJECT_SOURCE_DIR}/src/sh1371.c
  ${PROJECT_SOURCE_DIR}/src/sh1372.c
  ${PROJECT_SOURCE_DIR}/src/sh1373.c
  ${PROJECT_SOURCE_DIR}/src/sh1374.c
  ${PROJECT_SOURCE_DIR}/src/sh1375.c
  ${PROJECT_SOURCE_DIR}/src/sh1460.c
  ${PROJECT_SOURCE_DIR}/src/sh1461.c
  ${PROJECT_SOURCE_DIR}/src/sh1462.c
  ${PROJECT_SOURCE_DIR}/src/sh1463.c
  ${PROJECT_SOURCE_DIR}/src/sh1464.c
  ${PROJECT_SOURCE_DIR}/src/sh1465.c
  ${PROJECT_SOURCE_DIR}/src/sh1466.c
  ${PROJECT_SOURCE_DIR}/src/sh1467.c
  ${PROJECT_SOURCE_DIR}/src/sh1502.c
  ${PROJECT_SOURCE_DIR}/src/sh1503.c
  ${PROJECT_SOURCE_DIR}/src/sh1510.c
  ${PROJECT_SOURCE_DIR}/src/sh1511.c
  ${PROJECT_SOURCE_DIR}/src/sh1761.c
  ${PROJECT_SOURCE_DIR}/src/sh1762.c
  ${PROJECT_SOURCE_DIR}/src/sh1779.c
  ${PROJECT_SOURCE_DIR}/src/sh1779_at.c
  ${PROJECT_SOURCE_DIR}/src/sh1780.c
  ${PROJECT_SOURCE_DIR}/src/sh1780_at.c
  ${PROJECT_SOURCE_DIR}/src/sh1781.c
  ${PROJECT_SOURCE_DIR}/src/sh1781_at.c
  ${PROJECT_SOURCE_DIR}/src/sh1782.c
  ${PROJECT_SOURCE_DIR}/src/sh1783.c
  ${PROJECT_SOURCE_DIR}/src/sh1784.c
  ${PROJECT_SOURCE_DIR}/src/sh1786.c
  ${PROJECT_SOURCE_DIR}/src/sh1787.c
  ${PROJECT_SOURCE_DIR}/src/sh1790.c
  ${PROJECT_SOURCE_DIR}/src/sh1830.c
  ${PROJECT_SOURCE_DIR}/src/sh1831.c
  ${PROJECT_SOURCE_DIR}/src/sh1834.c
  ${PROJECT_SOURCE_DIR}/src/sh1839.c
  ${PROJECT_SOURCE_DIR}/src/sh1850.c
  ${PROJECT_SOURCE_DIR}/src/sh1851.c
  ${PROJECT_SOURCE_DIR}/src/sh1852.c
  ${PROJECT_SOURCE_DIR}/src/sh1853.c
  ${PROJECT_SOURCE_DIR}/src/sh1854.c
  ${PROJECT_SOURCE_DIR}/src/sh1855.c
  ${PROJECT_SOURCE_DIR}/src/sh1856.c
  ${PROJECT_SOURCE_DIR}/src/sh1857.c
  ${PROJECT_SOURCE_DIR}/src/sh1858.c
  ${PROJECT_SOURCE_DIR}/src/sh1859.c
  ${PROJECT_SOURCE_DIR}/src/sh1860.c
  ${PROJECT_SOURCE_DIR}/src/sh1870.c
  ${PROJECT_SOURCE_DIR}/src/sh1871.c
  ${PROJECT_SOURCE_DIR}/src/sh1922.c
  ${PROJECT_SOURCE_DIR}/src/sh1923.c
  ${PROJECT_SOURCE_DIR}/src/sh1924.c
  ${PROJECT_SOURCE_DIR}/src/sh1925.c
  ${PROJECT_SOURCE_DIR}/src/sh1926.c
  ${PROJECT_SOURCE_DIR}/src/sh1927.c
  ${PROJECT_SOURCE_DIR}/src/sh1928.c
  ${PROJECT_SOURCE_DIR}/src/sh1929.c
  ${PROJECT_SOURCE_DIR}/src/sh1930.c
  ${PROJECT_SOURCE_DIR}/src/sh1992.c
  ${PROJECT_SOURCE_DIR}/src/sh1993.c
  ${PROJECT_SOURCE_DIR}/src/sh1994.c
  ${PROJECT_SOURCE_DIR}/src/sh6clvert.c
  ${PROJECT_SOURCE_DIR}/src/sh6comedg.c
  ${PROJECT_SOURCE_DIR}/src/sh6connect.c
  ${PROJECT_SOURCE_DIR}/src/sh6count.c
  ${PROJECT_SOURCE_DIR}/src/sh6cvvert.c
  ${PROJECT_SOURCE_DIR}/src/sh6degen.c
  ${PROJECT_SOURCE_DIR}/src/sh6disconn.c
  ${PROJECT_SOURCE_DIR}/src/sh6edgpnt.c
  ${PROJECT_SOURCE_DIR}/src/sh6edgred.c
  ${PROJECT_SOURCE_DIR}/src/sh6evalint.c
  ${PROJECT_SOURCE_DIR}/src/sh6floop.c
  ${PROJECT_SOURCE_DIR}/src/sh6fndsplt.c
  ${PROJECT_SOURCE_DIR}/src/sh6getgeom.c
  ${PROJECT_SOURCE_DIR}/src/sh6getlist.c
  ${PROJECT_SOURCE_DIR}/src/sh6getmain.c
  ${PROJECT_SOURCE_DIR}/src/sh6getnbrs.c
  ${PROJECT_SOURCE_DIR}/src/sh6getnext.c
  ${PROJECT_SOURCE_DIR}/src/sh6getothr.c
  ${PROJECT_SOURCE_DIR}/src/sh6getprev.c
  ${PROJECT_SOURCE_DIR}/src/sh6gettop.c
  ${PROJECT_SOURCE_DIR}/src/sh6idaledg.c
  ${PROJECT_SOURCE_DIR}/src/sh6idcon.c
  ${PROJECT_SOURCE_DIR}/src/sh6idfcros.c
  ${PROJECT_SOURCE_DIR}/src/sh6idget.c
  ${PROJECT_SOURCE_DIR}/src/sh6idkpt.c
  ${PROJECT_SOURCE_DIR}/src/sh6idlis.c
  ${PROJECT_SOURCE_DIR}/src/sh6idnpt.c
  ${PROJECT_SOURCE_DIR}/src/sh6idnwun.c
  ${PROJECT_SOURCE_DIR}/src/sh6idput.c
  ${PROJECT_SOURCE_DIR}/src/sh6idrcros.c
  ${PROJECT_SOURCE_DIR}/src/sh6idsplit.c
  ${PROJECT_SOURCE_DIR}/src/sh6idunite.c
  ${PROJECT_SOURCE_DIR}/src/sh6insert.c
  ${PROJECT_SOURCE_DIR}/src/sh6inspnt.c
  ${PROJECT_SOURCE_DIR}/src/sh6iscnect.c
  ${PROJECT_SOURCE_DIR}/src/sh6ishelp.c
  ${PROJECT_SOURCE_DIR}/src/sh6isinsid.c
  ${PROJECT_SOURCE_DIR}/src/sh6ismain.c
  ${PROJECT_SOURCE_DIR}/src/sh6nmbhelp.c
  ${PROJECT_SOURCE_DIR}/src/sh6nmbmain.c
  ${PROJECT_SOURCE_DIR}/src/sh6ptobj.c
  ${PROJECT_SOURCE_DIR}/src/sh6ptouchp.c
  ${PROJECT_SOURCE_DIR}/src/sh6putsing.c
  ${PROJECT_SOURCE_DIR}/src/sh6red.c
  ${PROJECT_SOURCE_DIR}/src/sh6remcon.c
  ${PROJECT_SOURCE_DIR}/src/sh6rempnt.c
  ${PROJECT_SOURCE_DIR}/src/sh6sepcrv.c
  ${PROJECT_SOURCE_DIR}/src/sh6setcnsd.c
  ${PROJECT_SOURCE_DIR}/src/sh6setdir.c
  ${PROJECT_SOURCE_DIR}/src/sh6settop.c
  ${PROJECT_SOURCE_DIR}/src/sh6spltgeo.c
  ${PROJECT_SOURCE_DIR}/src/sh6tohelp.c
  ${PROJECT_SOURCE_DIR}/src/sh6tomain.c
  ${PROJECT_SOURCE_DIR}/src/sh6topohlp.c
  ${PROJECT_SOURCE_DIR}/src/sh6trmlist.c
  ${PROJECT_SOURCE_DIR}/src/sh_1d_div.c
  ${PROJECT_SOURCE_DIR}/src/sh_div_crv.c
  ${PROJECT_SOURCE_DIR}/src/sh_set_at.c
  ${PROJECT_SOURCE_DIR}/src/shape.c
  ${PROJECT_SOURCE_DIR}/src/shcheckput.c
  ${PROJECT_SOURCE_DIR}/src/shchecktyp.c
  ${PROJECT_SOURCE_DIR}/src/shcsfsing.c
  ${PROJECT_SOURCE_DIR}/src/shdivsurf.c
  ${PROJECT_SOURCE_DIR}/src/shevalc.c
  ${PROJECT_SOURCE_DIR}/src/shmkhlppts.c
  ${PROJECT_SOURCE_DIR}/src/shsing.c
  ${PROJECT_SOURCE_DIR}/src/spli_silh.c
  ${PROJECT_SOURCE_DIR}/src/tstcyclknt.c
)

IF(STATIC)
  IF(SHARED)
    add_library( ${LIB_NAME}_static STATIC ${PKG_SRC} )
    link_libraries( ${LIB_NAME}_static )
    install( TARGETS ${LIB_NAME}_static DESTINATION lib )
  ELSE(SHARED)
    add_library( ${LIB_NAME} STATIC ${PKG_SRC} )
    link_libraries( ${LIB_NAME} )
    install( TARGETS ${LIB_NAME} DESTINATION lib )
  ENDIF(SHARED)
ENDIF(STATIC)

IF(SHARED)
  add_library( ${LIB_NAME} SHARED ${PKG_SRC})
  link_libraries( ${LIB_NAME} )
  install( TARGETS ${LIB_NAME} DESTINATION lib )
ENDIF(SHARED)


ENDIF(LIBS)

######################################################################
# Programs
######################################################################
IF(APP)
ENDIF(APP)
IF(NOT USE_${PKG_NAME})
   file(WRITE "${CMAKE_BINARY_DIR}/local_paths" "export PATH=$PATH:${CMAKE_BINARY_DIR}/bin")
   file(APPEND "${CMAKE_BINARY_DIR}/local_paths" "
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${CMAKE_BINARY_DIR}/lib")
   file(APPEND "${CMAKE_BINARY_DIR}/local_paths" "
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:${CMAKE_BINARY_DIR}/lib")
   file(APPEND "${CMAKE_BINARY_DIR}/local_paths" "
export MMX_LOAD_PATH=$PWD:${CMAKE_SOURCE_DIR}:${CMAKE_SOURCE_DIR}/..:$MMX_LOAD_PATH")
ENDIF(NOT USE_${PKG_NAME})

ADD_CUSTOM_COMMAND(OUTPUT ${PROJECT_SOURCE_DIR}/.update
                   COMMAND svn up
                   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
ADD_CUSTOM_TARGET(update${PKG_SUFFIX}
                  DEPENDS ${PROJECT_SOURCE_DIR}/.update
                  COMMENT "update the packages")
IF(NOT USE_${PKG_NAME})
   file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/local_paths" "
##${PROJECT_SOURCE_DIR}/../mmxtools/bin/mmxmake $*")
   file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/local_paths" "
##${CMAKE_BINARY_DIR}/bin/mmx-light $*")
ENDIF(NOT USE_${PKG_NAME})
