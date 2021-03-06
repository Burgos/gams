
#include "com_gams_variables_Sensor.h"
#include "gams/variables/Sensor.h"
#include "gams/utility/GPSPosition.h"
#include "gams/pose/Region.h"
#include "gams/pose/SearchArea.h"

namespace containers = madara::knowledge::containers;
namespace engine = madara::knowledge;
namespace variables = gams::variables;
namespace pose = gams::pose;
namespace utility = gams::utility;

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_Sensor
 * Signature: ()J
 */
jlong JNICALL Java_com_gams_variables_Sensor_jni_1Sensor__
  (JNIEnv * , jobject)
{
  return (jlong) new variables::Sensor ();
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_Sensor
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_Sensor_jni_1Sensor__J
  (JNIEnv * , jobject, jlong cptr)
{
  return (jlong) new variables::Sensor (*(variables::Sensor *)cptr);
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_freeSensor
 * Signature: (J)V
 */
void JNICALL Java_com_gams_variables_Sensor_jni_1freeSensor
  (JNIEnv * , jclass, jlong cptr)
{
  delete (variables::Sensor *) cptr;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL Java_com_gams_variables_Sensor_jni_1getName
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  variables::Sensor * current = (variables::Sensor *) cptr;
  if (current)
  {
    result = env->NewStringUTF(current->get_name ().c_str ());
  }
  else
  {
    result = env->NewStringUTF ("");
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_init
 * Signature: (JJJLjava/lang/String;D)V
 */
void JNICALL Java_com_gams_variables_Sensor_jni_1init
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name, jdouble region)
{
  variables::Sensor * current = (variables::Sensor *) cptr;

  if (current)
  {
    double region_copy (region);
    const char * str_name = env->GetStringUTFChars(name, 0);

    if (type == 0)
    {
      engine::KnowledgeBase * kb = (engine::KnowledgeBase *) context;
      current->init_vars (str_name, kb, region_copy);
    }

    env->ReleaseStringUTFChars(name, str_name);
  }
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_toString
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL Java_com_gams_variables_Sensor_jni_1toString
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  variables::Sensor * current = (variables::Sensor *) cptr;
  if (current)
  {
    result = env->NewStringUTF(current->get_name ().c_str ());
  }
  else
  {
    result = env->NewStringUTF ("");
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_getPositionValue
 * Signature: (JJ)D
 */
jdouble JNICALL Java_com_gams_variables_Sensor_jni_1getPositionValue
  (JNIEnv *, jobject, jlong cptr, jlong coord_ptr)
{
  jdouble result (0.0);

  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::Position * coord = (pose::Position *) coord_ptr;
  if (current && coord)
  {
    result = current->get_value (*coord);
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_getGpsValue
 * Signature: (JJ)D
 */
jdouble JNICALL Java_com_gams_variables_Sensor_jni_1getGpsValue
  (JNIEnv *, jobject, jlong cptr, jlong coord_ptr)
{
  jdouble result (0.0);

  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::Position * coord = (pose::Position *) coord_ptr;
  if (current && coord)
  {
    result = current->get_value (*coord);
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_getOrigin
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_Sensor_jni_1getOrigin
  (JNIEnv *, jobject, jlong cptr)
{
  jdouble result (0.0);

  variables::Sensor * current = (variables::Sensor *) cptr;
  if (current)
  {
    result = (jlong) new utility::GPSPosition (current->get_origin ());
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_getRange
 * Signature: (J)D
 */
jdouble JNICALL Java_com_gams_variables_Sensor_jni_1getRange
  (JNIEnv *, jobject, jlong cptr)
{
  jdouble result (0.0);

  variables::Sensor * current = (variables::Sensor *) cptr;
  if (current)
  {
    result = current->get_range ();
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_setOrigin
 * Signature: (JJ)V
 */
void JNICALL Java_com_gams_variables_Sensor_jni_1setOrigin
  (JNIEnv *, jobject, jlong cptr, jlong coord_ptr)
{
  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::Position * coord = (pose::Position *) coord_ptr;
  if (current && coord)
  {
    current->set_origin (*coord);
  }
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_setRange
 * Signature: (JD)V
 */
void JNICALL Java_com_gams_variables_Sensor_jni_1setRange
  (JNIEnv *, jobject, jlong cptr, jdouble range)
{
  variables::Sensor * current = (variables::Sensor *) cptr;
  if (current)
  {
    current->set_range (range);
  }
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_setPositionValue
 * Signature: (JJD)V
 */
void JNICALL Java_com_gams_variables_Sensor_jni_1setPositionValue
  (JNIEnv *, jobject, jlong cptr, jlong coord_ptr, jdouble value)
{
  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::Position * coord = (pose::Position *) coord_ptr;
  if (current && coord)
  {
    current->set_value (*coord, value);
  }
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_setGpsValue
 * Signature: (JJD)V
 */
void JNICALL Java_com_gams_variables_Sensor_jni_1setGpsValue
  (JNIEnv *, jobject, jlong cptr, jlong coord_ptr, jdouble value)
{
  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::Position * coord = (pose::Position *) coord_ptr;
  if (current && coord)
  {
    current->set_value (*coord, value);
  }
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_getGpsFromIndex
 * Signature: (JJ)J
 */
jlong JNICALL Java_com_gams_variables_Sensor_jni_1getGpsFromIndex
  (JNIEnv *, jobject, jlong cptr, jlong index_ptr)
{
  jlong result (0);

  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::Position * index = (pose::Position *) index_ptr;
  if (current && index)
  {
    result = (jlong) new pose::Position (
      current->get_gps_from_index (*index));
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_getIndexFromGps
 * Signature: (JJ)J
 */
jlong JNICALL Java_com_gams_variables_Sensor_jni_1getIndexFromGps
  (JNIEnv *, jobject, jlong cptr, jlong coord_ptr)
{
  jlong result (0);

  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::Position * coord = (pose::Position *) coord_ptr;
  if (current && coord)
  {
    result = (jlong) new pose::Position (
      current->get_index_from_gps (*coord));
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_getDiscretization
 * Signature: (J)D
 */
jdouble JNICALL Java_com_gams_variables_Sensor_jni_1getDiscretization
  (JNIEnv *, jobject, jlong cptr)
{
  jdouble result (0.0);

  variables::Sensor * current = (variables::Sensor *) cptr;
  if (current)
  {
    result = current->get_discretization ();
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_discretizeRegion
 * Signature: (JJ)[J
 */
jlongArray JNICALL Java_com_gams_variables_Sensor_jni_1discretizeRegion
  (JNIEnv * env, jobject, jlong cptr, jlong region_ptr)
{
  jlongArray result (0);
  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::Region * region = (pose::Region *) region_ptr;
  
  if (current && region)
  {
    std::set<pose::Position> positions = current->discretize (*region);

    if (positions.size () > 0)
    {
      result = env->NewLongArray ((jsize)positions.size ());
      jlong * elements = env->GetLongArrayElements(result, 0);
      size_t cur (0);
      for (std::set<pose::Position>::iterator i = positions.begin ();
        i != positions.end (); ++i)
      {
        elements[cur] = (jlong) new utility::Position (*i);
      }
      env->ReleaseLongArrayElements(result, elements, 0);
    }
  }

  return result;
}

/*
 * Class:     com_gams_variables_Sensor
 * Method:    jni_discretizeSearchArea
 * Signature: (JJ)[J
 */
jlongArray JNICALL Java_com_gams_variables_Sensor_jni_1discretizeSearchArea
  (JNIEnv * env, jobject, jlong cptr, jlong area_ptr)
{
  jlongArray result (0);
  variables::Sensor * current = (variables::Sensor *) cptr;
  pose::SearchArea * area = (pose::SearchArea *) area_ptr;
  
  if (current && area)
  {
    std::set<pose::Position> positions = current->discretize (*area);

    if (positions.size () > 0)
    {
      result = env->NewLongArray ((jsize)positions.size ());
      jlong * elements = env->GetLongArrayElements(result, 0);
      size_t cur (0);
      for (std::set<pose::Position>::iterator i = positions.begin ();
        i != positions.end (); ++i)
      {
        elements[cur] = (jlong) new utility::Position (*i);
      }
      env->ReleaseLongArrayElements(result, elements, 0);
    }
  }

  return result;
}
