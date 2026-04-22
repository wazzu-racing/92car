#pragma once

// The main row struct.
typedef struct Row{
    int write_millis; // done
    int ecu_millis; // done
    int breakout_millis; // done
    int gps_millis; // done
    int imu_millis; // done
    int accel_millis; // done
    int analog_millis; // done
    int thermo_millis; // done

    int rpm; // done
    int time; // done
    int syncloss_count; // done
    int syncloss_code; // done
    int lat; // done
    int lon; // done
    int elev; // done
    int unixtime; // done
    int ground_speed; // done
    // int quickshift;
    // floats...
    int afr; // done
    int fuelload; // done
    int spark_advance; // done
    int baro; // done
    int map; // done
    int mat; // done
    int clnt_temp; // done
    int tps; // done
    int batt; // done
    int oil_press; // done
    int ltcl_timing; // done
    int ve1; // done
    int ve2; // done
    int maf; // done
    int in_temp; // done
    int ax; // done
    int ay; // done
    int az; // done
    int imu_x; // done
    int imu_y; // done
    int imu_z; // done
    int susp_pot_FL; // done
    int susp_pot_FR; // done
    int susp_pot_RL; // done
    int susp_pot_RR; // done
    int oil_temp; // done
    int amb_air_temp; // done
    int brake1; // done
    int brake2; // done
    int rad_in; // done
    int rad_out; // done
    int steering; // done
    int thermo_1; // done
    int thermo_2; // done
    int thermo_3; // done
    int thermo_4; // done
};
