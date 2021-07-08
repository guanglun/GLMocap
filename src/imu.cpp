#include "imu.h"
#include <QtMath>
#include "workspace.h"
#include "imuso3.h"

#define RtA 57.324841f
#define AtR 0.0174533f
#define Acc_G 0.0011963f
#define Gyro_G 0.0610351f
#define Gyro_Gr 0.0010653f
#define FILTER_NUM 2

//采样速度
#define Kp 10.0f      // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f     // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.0025f // half the sample period

IMU::IMU()
{
    setting->getIMUOffset(&ACC_OFFSET, &GYRO_OFFSET);
    startCalibration();
}

void IMU::startCalibration(void)
{
    DBG("start calibration");
    isYawCalStart = false;
    isGyroCalStart = false;
    isAccCalStart = false;
}

void IMU::calibration(T_int16_xyz *acc, T_int16_xyz *gyro)
{
    static T_int32_xyz tempg = {0, 0, 0}, tempa = {0, 0, 0};
    static unsigned char cnt_g = 0, cnt_a = 0;

    if (isGyroCalStart)
    {
        if (cnt_g == 0)
        {
            GYRO_OFFSET.X = 0;
            GYRO_OFFSET.Y = 0;
            GYRO_OFFSET.Z = 0;
            tempg.X = 0;
            tempg.Y = 0;
            tempg.Z = 0;
            cnt_g = 1;
        }
        else
        {
            tempg.X += gyro->X;
            tempg.Y += gyro->Y;
            tempg.Z += gyro->Z;
            if (cnt_g == 200)
            {
                GYRO_OFFSET.X = tempg.X / cnt_g;
                GYRO_OFFSET.Y = tempg.Y / cnt_g;
                GYRO_OFFSET.Z = tempg.Z / cnt_g;
                setting->setGyroOffset(&GYRO_OFFSET);

                DBG("calibration gyr %d,%d,%d", GYRO_OFFSET.X, GYRO_OFFSET.Y, GYRO_OFFSET.Z);

                cnt_g = 0;
                isGyroCalStart = false;
            }
            else
            {
                cnt_g++;
            }
        }
    }

    if (isAccCalStart)
    {
        if (cnt_a == 0)
        {
            ACC_OFFSET.X = 0;
            ACC_OFFSET.Y = 0;
            ACC_OFFSET.Z = 0;
            tempa.X = 0;
            tempa.Y = 0;
            tempa.Z = 0;
            cnt_a = 1;
        }
        else
        {
            tempa.X += acc->X;
            tempa.Y += acc->Y;
            //tempa.Z += acc->Z;
            if (cnt_a == 200)
            {
                ACC_OFFSET.X = tempa.X / cnt_a;
                ACC_OFFSET.Y = tempa.Y / cnt_a;
                //ACC_OFFSET.Z = tempa.Z/cnt_a;

                setting->setAccOffset(&ACC_OFFSET);

                DBG("calibration acc %d,%d,%d", ACC_OFFSET.X, ACC_OFFSET.Y, ACC_OFFSET.Z);
                cnt_a = 0;
                isAccCalStart = false;
            }
            else
            {
                cnt_a++;
            }
        }
    }
}

void IMU::recvData(unsigned char *imu_data, T_float_angle *angle, float time)
{
    Acc.X = (short)((imu_data[0] << 8) | imu_data[1]); // - ACC_OFFSET.X;
    Acc.Y = (short)((imu_data[2] << 8) | imu_data[3]); // - ACC_OFFSET.Y;
    Acc.Z = (short)((imu_data[4] << 8) | imu_data[5]); // - ACC_OFFSET.Z;

    Gyr.X = (short)((imu_data[6] << 8) | imu_data[7]);   //   - GYRO_OFFSET.X;
    Gyr.Y = (short)((imu_data[8] << 8) | imu_data[9]);   // - GYRO_OFFSET.Y;
    Gyr.Z = (short)((imu_data[10] << 8) | imu_data[11]); // - GYRO_OFFSET.Z;

    Mag.X = (short)((imu_data[12] << 8) | imu_data[13]);
    Mag.Y = (short)((imu_data[14] << 8) | imu_data[15]);
    Mag.Z = (short)((imu_data[16] << 8) | imu_data[17]);

    //temp = (short)((imu_data[12]<<8)|imu_data[13]);

    calibration(&Acc, &Gyr);
    prepareData(&Acc, &Acc_AVG);
    //update(&Gyr, &Acc_AVG, &Mag, angle, time);
    //IMUupdate(&Gyr, &Acc_AVG, &Mag, angle, time);
    IMUSO3Thread(&Gyr, &Acc_AVG, &Mag, angle, time);

}

void IMU::prepareData(T_int16_xyz *acc_in, T_int16_xyz *acc_out)
{
    static uint8_t filter_cnt = 0;
    static int16_t ACC_X_BUF[FILTER_NUM], ACC_Y_BUF[FILTER_NUM], ACC_Z_BUF[FILTER_NUM];
    int32_t temp1 = 0, temp2 = 0, temp3 = 0;
    uint8_t i;

    ACC_X_BUF[filter_cnt] = acc_in->X;
    ACC_Y_BUF[filter_cnt] = acc_in->Y;
    ACC_Z_BUF[filter_cnt] = acc_in->Z;
    for (i = 0; i < FILTER_NUM; i++)
    {
        temp1 += ACC_X_BUF[i];
        temp2 += ACC_Y_BUF[i];
        temp3 += ACC_Z_BUF[i];
    }
    acc_out->X = temp1 / FILTER_NUM;
    acc_out->Y = temp2 / FILTER_NUM;
    acc_out->Z = temp3 / FILTER_NUM;
    filter_cnt++;
    if (filter_cnt == FILTER_NUM)
        filter_cnt = 0;
}

void IMU::update(T_int16_xyz *gyr, T_int16_xyz *acc, T_int16_xyz *mag, T_float_angle *angle, float time)
{
    static float acc_cal = 9.8f * 8.0f / 65535 * 2;

    float gx, gy, gz, ax, ay, az;

    ax = acc->X * acc_cal;
    ay = acc->Y * acc_cal;
    az = acc->Z * acc_cal;

    gx = gyr->X * (500.0 / 65536.0) * (M_PI / 180.0);
    gy = gyr->Y * (500.0 / 65536.0) * (M_PI / 180.0);
    gz = gyr->Z * (500.0 / 65536.0) * (M_PI / 180.0);

    //DBG("%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%d\t%d",gx,gy,gz,ax,ay,az,time,mag->X,mag->Y,mag->Z);
    //DBG("%d\t%d\t%d", mag->X, mag->Y, mag->Z);

    float norm;
    //  float hx, hy, hz, bx, bz;
    float vx, vy, vz; // wx, wy, wz;
    float ex, ey, ez;

    // 先把这些用得到的值算好
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    //  float q0q3 = q0*q3;
    float q1q1 = q1 * q1;
    //  float q1q2 = q1*q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    if (ax * ay * az == 0)
        return;

    norm = sqrt(ax * ax + ay * ay + az * az); //acc数据归一化
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // estimated direction of gravity and flux (v and w)      估计重力方向和流量/变迁
    vx = 2 * (q1q3 - q0q2); //四元素中xyz的表示
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay * vz - az * vy); //向量外积在相减得到差分就是误差
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    exInt = exInt + ex * Ki; //对误差进行积分
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    // adjusted gyroscope measurements
    gx = gx + Kp * ex + exInt; //将误差PI后补偿到陀螺仪，即补偿零点漂移
    gy = gy + Kp * ey + eyInt;
    gz = gz + Kp * ez + ezInt; //这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

    // integrate quaternion rate and normalise						   //四元素的微分方程
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * time / 2;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * time / 2;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * time / 2;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * time / 2;

    // normalise quaternion
    norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    angle->yaw = qAtan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3; // yaw

    angle->pit = qAsin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;                                  // pitch
    angle->rol = -qAtan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // roll

    //DBG("%f\t%f\t%f",angle->yaw,angle->pit,angle->rol);

    if (isYawCalStart)
    {
        isYawCalStart = false;
        angle->yaw = 0;
    }
}

#define ANGLE_TO_RADIAN 0.01745329f //*0.01745 = /57.3	角度转弧度
#define Kp 0.3f                     // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.0f                     // 0.001  integral gain governs rate of convergence of gyroscope biases

#define IMU_INTEGRAL_LIM (2.0f * ANGLE_TO_RADIAN)
#define NORM_ACC_LPF_HZ 10 //(Hz)
#define REF_ERR_LPF_HZ 1   //(Hz)

#define TO_M_S2 				0.23926f   	//   980cm/s2    +-8g   980/4096

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )

xyz_f_t reference_v;
ref_t ref;

//xyz_f_t Gravity_Vec;  				//解算的重力向量

float Roll, Pitch, Yaw; //姿态角

float ref_q[4] = {1, 0, 0, 0};
float norm_acc, norm_q;
float norm_acc_lpf;

float mag_norm, mag_norm_xyz;

xyz_f_t mag_sim_3d, acc_3d_hg, acc_ng, acc_ng_offset;

uint8_t acc_ng_cali;
extern uint8_t fly_ready;

float To_180_degrees(float x)
{
    return (x>180?(x-360):(x<-180?(x+360):x));
}

void simple_3d_trans(xyz_f_t *ref, xyz_f_t *in, xyz_f_t *out) //小范围内正确。
{
    static char pn;
    static float h_tmp_x, h_tmp_y;

    h_tmp_x = sqrt(qPow(ref->z, 2) + qPow(ref->y, 2));
    h_tmp_y = sqrt(qPow(ref->z, 2) + qPow(ref->x, 2));

    pn = ref->z < 0 ? -1 : 1;

    out->x = (h_tmp_x * in->x - pn * ref->x * in->z);
    out->y = (pn * h_tmp_y * in->y - ref->y * in->z);

    // 	 out->x = h_tmp_x *in->x - ref->x *in->z;
    // 	 out->y = ref->z *in->y - ref->y *in->z;

    out->z = ref->x * in->x + ref->y * in->y + ref->z * in->z;
}

void IMU::IMUupdate(T_int16_xyz *gyr, T_int16_xyz *acc, T_int16_xyz *mag, T_float_angle *angle, float time)
//(float half_T, float gx, float gy, float gz, float ax, float ay, float az, float *rol, float *pit, float *yaw)
{
    float ref_err_lpf_hz;
    static float yaw_correct;
    float mag_norm_tmp;
    static xyz_f_t mag_tmp;
    static float yaw_mag;

    float gx, gy, gz;
    float ax, ay, az;

    static float acc_cal = 9.8f * 8.0f / 65535 * 2;

    ax = acc->X / 1;// * acc_cal;
    ay = acc->Y / 1;// * acc_cal;
    az = acc->Z / 1;// * acc_cal;

    gx = gyr->X * (4000.0 / 65536.0);
    gy = gyr->Y * (4000.0 / 65536.0);
    gz = gyr->Z * (4000.0 / 65536.0);

    //DBG("%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%d\t%d",gx,gy,gz,ax,ay,az,time,mag->X,mag->Y,mag->Z);
    //DBG("%d\t%d\t%d", mag->X, mag->Y, mag->Z);

    //DBG("%f",gz);
    mag_norm_tmp = 20 * (6.28f * time / 2);

    mag_norm_xyz = sqrt(mag->X * mag->X + mag->Y * mag->Y + mag->Z * mag->Z);

    if (mag_norm_xyz != 0)
    {
        mag_tmp.x += mag_norm_tmp * ((float)mag->X / (mag_norm_xyz)-mag_tmp.x);
        mag_tmp.y += mag_norm_tmp * ((float)mag->Y / (mag_norm_xyz)-mag_tmp.y);
        mag_tmp.z += mag_norm_tmp * ((float)mag->Z / (mag_norm_xyz)-mag_tmp.z);
    }

    DBG("%f\t%f\t%f", mag_tmp.x, mag_tmp.y, mag_tmp.z);

    /*
	void simple_3d_trans(_xyz_f_t *ref, _xyz_f_t *in, _xyz_f_t *out)
	
	罗盘数据是机体坐标下的，且磁场方向不是平行于地面，如果飞机倾斜，投影计算的角度会存在误差。
	此函数可在一定范围内做近似转换，让结果逼近实际角度，减小飞机倾斜的影响。
	注意：该函数内的计算并不是正确也不是准确的，正确的计算相对复杂，这里不给出，在未来的版本中会再更新。
	*/
    simple_3d_trans(&reference_v, &mag_tmp, &mag_sim_3d);

    mag_norm = sqrt(mag_sim_3d.x * mag_sim_3d.x + mag_sim_3d.y * mag_sim_3d.y);

    if (mag_sim_3d.x != 0 && mag_sim_3d.y != 0 && mag_sim_3d.z != 0 && mag_norm != 0)
    {
        yaw_mag = qAtan2((mag_sim_3d.y / mag_norm), (mag_sim_3d.x / mag_norm)) * 57.3f;
    }
    //=============================================================================
    // 计算等效重力向量
    reference_v.x = 2 * (ref_q[1] * ref_q[3] - ref_q[0] * ref_q[2]);
    reference_v.y = 2 * (ref_q[0] * ref_q[1] + ref_q[2] * ref_q[3]);
    reference_v.z = 1 - 2 * (ref_q[1] * ref_q[1] + ref_q[2] * ref_q[2]); //ref_q[0]*ref_q[0] - ref_q[1]*ref_q[1] - ref_q[2]*ref_q[2] + ref_q[3]*ref_q[3];

    //这是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。
    //根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素。
    //所以这里的vx\y\z，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的重力单位向量。
    //=============================================================================

    if (acc_ng_cali)
    {
        if (acc_ng_cali == 2)
        {
            acc_ng_offset.x = 0;
            acc_ng_offset.y = 0;
            acc_ng_offset.z = 0;
        }

        acc_ng_offset.x += 10 * TO_M_S2 * (ax - 4096 * reference_v.x) * 0.0125f;
        acc_ng_offset.y += 10 * TO_M_S2 * (ay - 4096 * reference_v.y) * 0.0125f;
        acc_ng_offset.z += 10 * TO_M_S2 * (az - 4096 * reference_v.z) * 0.0125f;

        acc_ng_cali++;
        if (acc_ng_cali >= 82) //start on 2
        {
            acc_ng_cali = 0;
        }
    }

    acc_ng.x = 10 * TO_M_S2 * (ax - 4096 * reference_v.x) - acc_ng_offset.x;
    acc_ng.y = 10 * TO_M_S2 * (ay - 4096 * reference_v.y) - acc_ng_offset.y;
    acc_ng.z = 10 * TO_M_S2 * (az - 4096 * reference_v.z) - acc_ng_offset.z;

    acc_3d_hg.z = acc_ng.x * reference_v.x + acc_ng.y * reference_v.y + acc_ng.z * reference_v.z;

    // 计算加速度向量的模
    norm_acc = sqrt(ax * ax + ay * ay + az * az);

    if ((qAbs(ax) < 4400) && (qAbs(ay) < 4400) && (qAbs(az) < 4400))
    {
        //把加计的三维向量转成单位向量。
        ax = ax / norm_acc; //4096.0f;
        ay = ay / norm_acc; //4096.0f;
        az = az / norm_acc; //4096.0f;

        if (3800 < norm_acc && norm_acc < 4400)
        {
            /* 叉乘得到误差 */
            ref.err_tmp.x = ay * reference_v.z - az * reference_v.y;
            ref.err_tmp.y = az * reference_v.x - ax * reference_v.z;
            //ref.err_tmp.z = ax*reference_v.y - ay*reference_v.x;

            /* 误差低通 */
            ref_err_lpf_hz = REF_ERR_LPF_HZ * (6.28f * time / 2);
            ref.err_lpf.x += ref_err_lpf_hz * (ref.err_tmp.x - ref.err_lpf.x);
            ref.err_lpf.y += ref_err_lpf_hz * (ref.err_tmp.y - ref.err_lpf.y);
            //			 ref.err_lpf.z += ref_err_lpf_hz *( ref.err_tmp.z  - ref.err_lpf.z );

            ref.err.x = ref.err_lpf.x; //
            ref.err.y = ref.err_lpf.y; //
                                       //				ref.err.z = ref.err_lpf.z ;
        }
    }
    else
    {
        ref.err.x = 0;
        ref.err.y = 0;
        //		ref.err.z = 0 ;
    }
    /* 误差积分 */
    ref.err_Int.x += ref.err.x * Ki * 2 * time / 2;
    ref.err_Int.y += ref.err.y * Ki * 2 * time / 2;
    ref.err_Int.z += ref.err.z * Ki * 2 * time / 2;

    /* 积分限幅 */
    ref.err_Int.x = LIMIT(ref.err_Int.x, -IMU_INTEGRAL_LIM, IMU_INTEGRAL_LIM);
    ref.err_Int.y = LIMIT(ref.err_Int.y, -IMU_INTEGRAL_LIM, IMU_INTEGRAL_LIM);
    ref.err_Int.z = LIMIT(ref.err_Int.z, -IMU_INTEGRAL_LIM, IMU_INTEGRAL_LIM);

    if (reference_v.z > 0.0f)
    {
//        if (fly_ready)
//        {
//            yaw_correct = Kp * 0.2f * To_180_degrees(yaw_mag - Yaw);
//            //已经解锁，只需要低速纠正。
//        }
//        else
//        {
            yaw_correct = Kp * 1.5f * To_180_degrees(yaw_mag - Yaw);
//            //没有解锁，视作开机时刻，快速纠正
//        }
        // 		if( yaw_correct>360 || yaw_correct < -360  )
        // 		{
        // 			yaw_correct = 0;
        // 			//限制纠正范围+-360，配合+-180度取值函数
        // 		}
    }
    else
    {
        yaw_correct = 0; //角度过大，停止修正，修正的目标值可能不正确
    }

    //DBG("%f",yaw_correct);

    ref.g.x = (gx - reference_v.x * yaw_correct) * ANGLE_TO_RADIAN + (Kp * (ref.err.x + ref.err_Int.x)); //IN RADIAN
    ref.g.y = (gy - reference_v.y * yaw_correct) * ANGLE_TO_RADIAN + (Kp * (ref.err.y + ref.err_Int.y)); //IN RADIAN
    ref.g.z = (gz - reference_v.z * yaw_correct) * ANGLE_TO_RADIAN;

    /* 用叉积误差来做PI修正陀螺零偏 */

    // integrate quaternion rate and normalise
    ref_q[0] = ref_q[0] + (-ref_q[1] * ref.g.x - ref_q[2] * ref.g.y - ref_q[3] * ref.g.z) * time / 2;
    ref_q[1] = ref_q[1] + (ref_q[0] * ref.g.x + ref_q[2] * ref.g.z - ref_q[3] * ref.g.y) * time / 2;
    ref_q[2] = ref_q[2] + (ref_q[0] * ref.g.y - ref_q[1] * ref.g.z + ref_q[3] * ref.g.x) * time / 2;
    ref_q[3] = ref_q[3] + (ref_q[0] * ref.g.z + ref_q[1] * ref.g.y - ref_q[2] * ref.g.x) * time / 2;

    /* 四元数规一化 normalise quaternion */
    norm_q = sqrt(ref_q[0] * ref_q[0] + ref_q[1] * ref_q[1] + ref_q[2] * ref_q[2] + ref_q[3] * ref_q[3]);
    ref_q[0] = ref_q[0] / norm_q;
    ref_q[1] = ref_q[1] / norm_q;
    ref_q[2] = ref_q[2] / norm_q;
    ref_q[3] = ref_q[3] / norm_q;

    angle->rol = qAtan2(2 * (ref_q[0] * ref_q[1] + ref_q[2] * ref_q[3]), 1 - 2 * (ref_q[1] * ref_q[1] + ref_q[2] * ref_q[2])) * 57.3f;
    angle->pit = qAsin(2 * (ref_q[1] * ref_q[3] - ref_q[0] * ref_q[2])) * 57.3f;

    angle->yaw = qAtan2(2 * (-ref_q[1] * ref_q[2] - ref_q[0] * ref_q[3]), 2 * (ref_q[0] * ref_q[0] + ref_q[1] * ref_q[1]) - 1) * 57.3f;
    //angle->yaw = yaw_correct;//yaw_mag;

}
