#define     PI               3.141592653589793
#define     TWO_PI           (2.0 * PI)
#define     PI_OVER_3        (PI / 3.0)
#define     TWO_PI_OVER_3    (2.0 * PI_OVER_3)
#define     PI_OVER_2        (PI / 2.0)
#define     THREE_PI_OVER_2  (3.0 * PI_OVER_2)
#define     PI_OVER_4        (PI / 4.0)
#define     THREE_PI_OVER_4  (3.0 * PI_OVER_4)
    /* Pi(s) and pieces of Pi */

#define     SQUARE_ROOT_OF_2   1.414214
    /* The square root of 2 */

#define    ONE_REVOLUTION  360.0
    /* 360.0 degrees */

#define    THREE_FOURTHS_REVOLUTION 270.0
    /* 270.0 degrees */

#define    ONE_HALF_REVOLUTION  180.0
    /* 180.0 degrees */

#define    ONE_QUARTER_REVOLUTION 90.0
    /* 90.0 degrees */

    /* Angle Conversion Factors */

#define    DEGREES_TO_RADIANS         (PI / 180.0)
#define    RADIANS_TO_DEGREES         (180.0 / PI)

#define    MINUTES_TO_RADIANS         (DEGREES_TO_RADIANS / 60.0)

#define    SECONDS_TO_RADIANS         (DEGREES_TO_RADIANS / (60.0 * 60.0))

#define    ACPS_TO_RADIANS            (TWO_PI / 4096.0)
#define    RADIANS_TO_ACPS            (4096.0 / TWO_PI)

#define    ACPS_TO_DEGREE             (360.0 / 4096.0)
#define    DEGREES_TO_ACPS            (4096.0 / 360.0)


    /* Circle conversion constants */

#define    CIRCLES_TO_ACPS            4096.0
#define    ACPS_TO_CIRCLES            (1.0 /CIRCLES_TO_ACPS) 

#define    CIRCLES_TO_RADIANS         TWO_PI
#define    RADIANS_TO_CIRCLES         (1.0 / CIRCLES_TO_RADIANS)


    /* Velocity conversion constants */

#define    KNOTS_TO_NM_PER_SECOND     (1.0 / 3600.0)
#define    NM_PER_SECOND_TO_KNOTS     3600.0


    /* Range conversion constants */

#define		KM_TO_NM                   (1.0 / 1.852)
#define		MT_TO_NM					(1.0/1000.0/1.852)
#define  MAX_DAY_NUMBER							767009 
/* number of days; allows for 2099 years.                                   */

#define  MIN_DAY_NUMBER						(-MAX_DAY_NUMBER )
/* number of days.                                                          */

#define  NBR_OF_MILLISECONDS_IN_A_DAY			86400000 
/* number of milliseconds in a day.                                         */
/* units of measure: milliseconds                                           */

#define  NBR_OF_MILLISECONDS_IN_AN_HOUR			3600000 
/* number of milliseconds in an hour.                                       */
/* units of measure: milliseconds                                           */

#define  NBR_OF_MILLISECONDS_IN_A_MINUTE		60000 
/* number of milliseconds in a minute.                                      */
/* units of measure: milliseconds                                           */

#define  NBR_OF_MILLISECONDS_IN_A_SECOND		1000 
/* number of milliseconds in a second.                                      */
/* units of measure: milliseconds                                           */

#define  NBR_OF_SECONDS_IN_AN_HOUR				3600
/* number of seconds in an hour.                                            */
/* units of measure: seconds                                                */

#define  NBR_OF_SECONDS_IN_A_MINUTE				60 
/* number of seconds in a minute.                                           */
/* units of measure: seconds												*/


                                                          
  /* distance conversion factors                                              */

#define DM_TO_FEET								6000.0
  /* data miles to feet conversion factor                                     */

#define FEET_TO_DM								(1.0 / DM_TO_FEET)
  /* feet to data miles conversion factor                                     */

#define METERS_TO_FEET							3.2808
  /* meters to feet conversion factor                                         */

#define FEET_TO_METERS							(1.0 / METERS_TO_FEET)
  /* feet to meters conversion factor                                         */

#define NMI_TO_FEET								6076.115
  /* nautical miles to feet conversion factor                                 */

#define FEET_TO_NMI								(1.0 / NMI_TO_FEET)
  /* feet to nautical miles conversion factor                                 */

#define NM_TO_KILO_METERS						1.852
  /* nautical miles to kilometers conversion factor                           */

#define TENS_OF_METERS_TO_TENS_OF_NMI			18.52
  /* tens of meters to tens of nautical miles conversion factor               */

#define TENS_OF_METERS_TO_TENS_OF_MI			16.09
  /* tens of meters to tenth of miles conversion factor                       */

#define KILO_METERS_TO_NM						(1.0 / NM_TO_KILO_METERS)
  /* kilometers to nautical miles conversion factor                           */

#define NM_TO_DM								1.012685914
  /* nautical miles to data miles conversion factor                           */

#define DM_TO_NM								(1.0 / NM_TO_DM)	//필요
  /* data miles to nautical miles conversion factor                           */

#define NM_TO_FEET								(NM_TO_DM * DM_TO_FEET)
  /* nautical miles to feet conversion factor                                 */

#define FEET_TO_NM								(1.0 / NM_TO_FEET)
  /* feet to nautical miles conversion factor                                 */

#define NM_TO_YARD								(NM_TO_FEET / 3.0)
  /* nautical miles to yard conversion factor                                 */

#define YARD_TO_NM								(1.0 / NM_TO_YARD)	//필요
  /* yard to nautical miles conversion factor                                 */

#define YARD_TO_FEET							3.0
  /* yard to feet conversion factor                                           */

#define FEET_TO_YARD							(1.0 / YARD_TO_FEET)
  /* feet to yard conversion factor                                           */

#define MAJOR_GRID_TO_MINUTES					900
  /* 15 deg/grid * 60 min/deg                                                 */

#define MINOR_GRID_TO_MINTES					60
  /* 60 min/deg                                                               */

#define DEGREES_TO_MINUTES						60
  /* 60 min/deg                                                               */

#define DEGREES_PER_MAJOR_GRID					15
  /* 15 deg/major grid                                                        */

#define DEGREE_TO_MINUTES						60.0
  /* 60.0 min/deg                                                             */

#define MINUTE_TO_DEGREES						1.0 / DEGREE_TO_MINUTES

#define MINUTE_TO_SECONDS						60.0
  /* 60.0 sec/min                                                             */

#define SECOND_TO_MINUTES						1.0 / MINUTE_TO_SECONDS

#define DEGREE_TO_SECONDS						3600.0
  /* 3600.0 sec/deg                                                           */

#define SECOND_TO_DEGREES						1.0 / DEGREE_TO_SECONDS


