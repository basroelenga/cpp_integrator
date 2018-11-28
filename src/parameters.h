#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <vector>

/* Forward declare the gas object class */
class Gas_object;

class Parameters
{

  private:

    /* MPI properties */
    int world_size;
    int world_rank;

    /* Time integration options */
    double total_time;
    double dt;

    /* Initial conditions data */
    std::string input_data;

    /* Snapshot options */
    std::vector<double> snap_times;

    bool output_dust_gas;
    bool repack;

    /* Target data */
    std::vector<std::string> targets;
    std::vector<int> target_indx_list;

    double r_cut;

    /* Debug info */
    int num_objects;

    /* Gas object list, for now this is part of the parameter object */
    std::shared_ptr<std::vector<Gas_object>> gas_obj_list;

    // Centauri indx
    std::vector<int> centauri_indx;

    /* Load functions */
    void load_parameter_file(std::string param_name);
    void load_target_file(std::string cutoff_name);
    void load_snapshot_file(std::string snap_name);

  public:

    Parameters(int world_size, int world_rank);

    double get_total_time();
    double get_dt();

    int get_world_size();
    int get_world_rank();

    uint get_snapshots_num();
    int get_input_mode();

    void set_cut_indx(int indx);
    int get_cut_indx();

    double get_r_cut();

    std::string get_output_name();
    std::string get_input_data();

    std::vector<std::string> get_targets();

    void set_target_indx(int indx);
    std::vector<int> get_target_indx_list();

    double get_snap_time(int s);

    int get_num_objects();
    void set_num_objects(int num_objects);

    bool should_repack();
    bool should_output_dust_gas();

    void add_gas_object(Gas_object g_obj);
    std::shared_ptr<std::vector<Gas_object>> get_gas_objects();

    std::vector<int> get_centuari_indx();
};

#endif
