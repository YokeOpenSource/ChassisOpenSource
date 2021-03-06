#include "pid.h"
#include "math.h"


/**
  * @brief     limit the max value
  * @param[in] *a: the data that need to limit
  * @param[in] ABS_MAX: limit value
  * @retval    none
  */
void abs_limit(float *a, float ABS_MAX)

{

  if (*a > ABS_MAX)

    *a = ABS_MAX;

  if (*a < -ABS_MAX)

    *a = -ABS_MAX;

}

 
/**
  * @brief     init the p/i/d param
  * @param[in] pid: the pid struct point
  * @param[in] mode: position or delta
  * @param[in] maxout: the max output value
  * @param[in] intergral_limit: the limit value of intergral
 * @param[in] kp/ki/kd param
  * @retval    none
  */
static void pid_param_init(

    pid_t*   pid,

    uint32_t mode,

    uint32_t maxout,

    uint32_t intergral_limit,

    float    kp,

    float    ki,

    float    kd)

{
  pid->integral_limit = intergral_limit;

  pid->max_out        = maxout;

  pid->pid_mode       = mode;



  pid->p = kp;

  pid->i = ki;

  pid->d = kd;



}

/**
  * @brief     modify pid parameter when code running
  * @param[in] pid: control pid struct
  * @param[in] p/i/d: pid parameter
  * @retval    none
  */

static void pid_reset(pid_t *pid, float kp, float ki, float kd)

{

  pid->p = kp;

  pid->i = ki;

  pid->d = kd;

  

  pid->pout = 0;

  pid->iout = 0;

  pid->dout = 0;

  pid->out  = 0;

  

}



/**
  * @brief     calculate delta PID and position PID
  * @param[in] pid: control pid struct
  * @param[in] get: measure feedback value
  * @param[in] set: target value
  * @retval    pid calculate output 
  */

float pid_calc(pid_t *pid,int16_t set,int16_t get)

{
  pid->err[NOW] = set - get;

//  if ((pid->input_max_err != 0) && (fabs(pid->err[NOW]) > pid->input_max_err))

//      return 0;



  if (pid->pid_mode == POSITION_PID) //position PID

  {

      pid->pout = pid->p * pid->err[NOW];

      pid->iout += pid->i * pid->err[NOW];

      pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]);

    

      abs_limit(&(pid->iout), pid->integral_limit);

      pid->out = pid->pout + pid->iout + pid->dout;

      abs_limit(&(pid->out), pid->max_out);

  }

  else if (pid->pid_mode == DELTA_PID) //delta PID

  {

      pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);

      pid->iout = pid->i * pid->err[NOW];

      pid->dout = pid->d * (pid->err[NOW] - 2 * pid->err[LAST] + pid->err[LLAST]);

      pid->out += pid->pout + pid->iout + pid->dout;

      abs_limit(&(pid->out), pid->max_out);
	  
	  if( set > 0 && pid->out > 0 && pid->out < 45)
	  {
		   pid->out = 45;
	  }
	  else if(set > 0 && pid->out > -45 && pid->out <= 0)
	  {
		   pid->out = 45;
	  }
	  
	  if( set < 0 && pid->out > 0 && pid->out < 45)
	  {
		   pid->out = -45;
	  }
	  else if(set < 0 && pid->out > -45 && pid->out <= 0)
	  {
		   pid->out = -45;
	  }	  

  }
  pid->err[LLAST] = pid->err[LAST];

  pid->err[LAST]  = pid->err[NOW];

  

   

  if ((pid->output_deadband != 0) && (fabs(pid->out) < pid->output_deadband))

    return 0;

  else

    return pid->out;



}

/**
  * @brief     initialize pid parameter
  * @retval    none
  */

void PID_struct_init(

    pid_t*   pid,

    uint32_t mode,

    uint32_t maxout,

    uint32_t intergral_limit,

    float kp,

    float ki,

    float kd)

{

  pid->f_param_init = pid_param_init;

  pid->f_pid_reset  = pid_reset;



  pid->f_param_init(pid, mode, maxout, intergral_limit, kp, ki, kd);

  pid->f_pid_reset(pid, kp, ki, kd);

}
