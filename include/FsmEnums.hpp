#ifndef fsm_enums_h
#define fsm_enums_h
enum State
{
    SETUP,
    MM,
    SETTINGS,
    ADDEDIT,
    DELLIST,
    DEL,
    SENSOR,
    CLOUD,
    UPLOAD,
    DOWNLOAD,
    CHECK,
    s_name,
    s_pin,
    s_voltage,
    s_resistance,
    s_fc
};
enum Event
{
    e_setup,
    e_mm,
    e_cloud,
    e_up,
    e_down,
    e_cs,
    e_sett,
    e_addedit,
    e_dellist,
    e_del,
    e_check,
    e_name,
    e_pin,
    e_voltage,
    e_resistance,
    e_fc
};
#endif