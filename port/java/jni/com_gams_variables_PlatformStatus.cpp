
#include "com_gams_variables_PlatformStatus.h"
#include "gams/variables/PlatformStatus.h"

namespace containers = madara::knowledge::containers;
namespace engine = madara::knowledge;
namespace variables = gams::variables;

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_PlatformStatus
 * Signature: ()J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1PlatformStatus__
  (JNIEnv * , jobject)
{
  return (jlong) new variables::PlatformStatus ();
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_PlatformStatus
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1PlatformStatus__J
  (JNIEnv * , jobject, jlong cptr)
{
  return (jlong) new variables::PlatformStatus (
    *(variables::PlatformStatus *)cptr);
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_freePlatformStatus
 * Signature: (J)V
 */
void JNICALL Java_com_gams_variables_PlatformStatus_jni_1freePlatformStatus
  (JNIEnv * , jclass, jlong cptr)
{
  delete (variables::PlatformStatus *) cptr;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL Java_com_gams_variables_PlatformStatus_jni_1getName
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;
  if (current)
  {
    result = env->NewStringUTF(current->name.c_str ());
  }
  else
  {
    result = env->NewStringUTF ("");
  }

  return result;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_init
 * Signature: (JJJLjava/lang/String;)V
 */
void JNICALL Java_com_gams_variables_PlatformStatus_jni_1init
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  if (current)
  {
    const char * str_name = env->GetStringUTFChars(name, 0);

    if (type == 0)
    {
      engine::KnowledgeBase * kb = (engine::KnowledgeBase *) context;
      current->init_vars (*kb, str_name);
    }
    else if (type == 1)
    {
      engine::Variables * vars = (engine::Variables *) context;
      current->init_vars (*vars, str_name);
    }

    env->ReleaseStringUTFChars(name, str_name);
  }
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_toString
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL Java_com_gams_variables_PlatformStatus_jni_1toString
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;
  if (current)
  {
    result = env->NewStringUTF(current->name.c_str ());
  }
  else
  {
    result = env->NewStringUTF ("");
  }

  return result;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getCommunicationAvailable
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getCommunicationAvailable
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->communication_available;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getDeadlocked
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getDeadlocked
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->deadlocked;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getFailed
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getFailed
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->failed;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getGpsSpoofed
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getGpsSpoofed
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->gps_spoofed;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getMovementAvailable
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getMovementAvailable
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->movement_available;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getMoving
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getMoving
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->moving;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getOk
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getOk
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->ok;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getPausedMoving
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getPausedMoving
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->paused_moving;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getReducedSensing
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getReducedSensing
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->reduced_sensing;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getReducedMovement
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getReducedMovement
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->reduced_movement;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getSensorsAvailable
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getSensorsAvailable
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->sensors_available;
}

/*
 * Class:     com_gams_variables_PlatformStatus
 * Method:    jni_getWaiting
 * Signature: (J)J
 */
jlong JNICALL Java_com_gams_variables_PlatformStatus_jni_1getWaiting
  (JNIEnv * , jobject, jlong cptr)
{
  variables::PlatformStatus * current = (variables::PlatformStatus *) cptr;

  return (jlong) &current->waiting;
}
