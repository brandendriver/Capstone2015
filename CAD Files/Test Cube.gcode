; KISSlicer - FREE
; Apple 10.9.5
; version 1.4.5.10
; Built: Sep  2 2014, 20:48:42
; Running on 2 cores
;
; Saved: Fri May  1 14:49:30 2015
; 'Test Cube.gcode'
;
; *** Printer Settings ***
;
; printer_name = sample printer
; bed_STL_filename = 
; extension = gcode
; cost_per_hour = 0
; g_code_prefix = 25
; g_code_warm = 0A
; g_code_cool = 0A
; g_code_N_layers = 3B204D617962652072652D686F6D65205820262059
;     3F0A
; g_code_postfix = 25
; post_process = NULL
; every_N_layers = 0
; num_extruders = 1
; firmware_type = 1
; add_comments = 1
; fan_on = M106
; fan_off = M107
; fan_pwm = 1
; add_m101_g10 = 0
; z_speed_mm_per_s = 3.5
; z_settle_mm = 0.25
; bed_size_x_mm = 100
; bed_size_y_mm = 100
; bed_size_z_mm = 100
; ext_radius = 0
; bed_offset_x_mm = 0
; bed_offset_y_mm = 0
; bed_offset_z_mm = 0
; bed_roughness_mm = 0.25
; round_bed = 0
; travel_speed_mm_per_s = 500
; first_layer_speed_mm_per_s = 10
; dmax_per_layer_mm_per_s = 50
; xy_accel_mm_per_s_per_s = 1500
; xy_steps_per_mm = 150
; lo_speed_perim_mm_per_s = 10
; lo_speed_solid_mm_per_s = 10
; lo_speed_sparse_mm_per_s = 10
; hi_speed_perim_mm_per_s = 30
; hi_speed_solid_mm_per_s = 30
; hi_speed_sparse_mm_per_s = 30
; ext_gain_1 = 1
; ext_material_1 = 0
; ext_axis_1 = 0
; ext_gain_2 = 1
; ext_material_2 = 0
; ext_axis_2 = 0
; ext_Xoff_2 = 0
; ext_Yoff_2 = 0
; ext_gain_3 = 1
; ext_material_3 = 0
; ext_axis_3 = 0
; ext_Xoff_3 = 0
; ext_Yoff_3 = 0
; ext_gain_4 = 1
; ext_material_4 = 0
; ext_axis_4 = 0
; ext_Xoff_4 = 0
; ext_Yoff_4 = 0
; model_ext = 0
; support_ext = 0
; support_body_ext = 0
; raft_ext = 0
; solid_loop_overlap_fraction = 0.5
;
; *** Material Settings for Extruder 1 ***
;
; material_name = sample material
; g_code_matl = 3B204D617962652073657420736F6D65206D6174657269
;     616C2D737065636966696320472D636F64653F
; fan_Z_mm = 0
; fan_loops_percent = 100
; fan_inside_percent = 0
; fan_cool_percent = 100
; temperature_C = 250
; keep_warm_C = 180
; first_layer_C = 255
; bed_C = 80
; sec_per_C_per_C = 0
; flow_min_mm3_per_s = 0.01
; flow_max_mm3_per_s = 10
; destring_suck = 1.25
; destring_prime = 1.25
; destring_min_mm = 1
; destring_trigger_mm = 100
; destring_speed_mm_per_s = 15
; Z_lift_mm = 0
; min_layer_time_s = 10
; wipe_mm = 10
; cost_per_cm3 = 0
; flowrate_tweak = 1
; fiber_dia_mm = 3
; color = 0
;
; *** Style Settings ***
;
; style_name = sample style
; layer_thickness_mm = 0.25
; extrusion_width_mm = 0.5
; num_loops = 3
; skin_thickness_mm = 0.8
; infill_extrusion_width = 0.5
; infill_density_denominator = 5
; stacked_layers = 1
; use_destring = 1
; use_wipe = 1
; loops_insideout = 0
; infill_st_oct_rnd = 1
; inset_surface_xy_mm = 0
; seam_jitter_degrees = 0
; seam_depth_scaler = 1
; seam_gap_scaler = 1
; seam_angle_degrees = 45
;
; *** Support Settings ***
;
; support_name = sample support
; support_sheathe = 0
; support_density = 3
; solid_interface = 0
; support_inflate_mm = 0
; support_gap_mm = 0.5
; support_angle_deg = 45
; support_z_max_mm = -1
; sheathe_z_max_mm = -1
; raft_mode = 0
; prime_pillar_mode = 0
; raft_inflate_mm = 2
; brim_mm = 0
; brim_ht_mm = 0
; brim_fillet = 1
;
; *** Actual Slicing Settings As Used ***
;
; layer_thickness_mm = 0.25
; extrusion_width = 0.5
; num_ISOs = 3
; wall_thickness = 0.8
; infill_style = 6
; support_style = 3
; solid_interface = 0
; support_angle = 44.9
; destring_min_mm = 1
; stacked_infill_layers = 1
; raft_style = 0
; extra_raft_depth = 0.25
; oversample_res_mm = 0.125
; crowning_threshold_mm = 1
; loops_insideout = 0
; solid_loop_overlap_fraction = 0.5
; inflate_raft_mm = 0
; inflate_support_mm = 0
; model_support_gap_mm = 0.5
; brim_mm = 0
; brim_ht_mm = 0
; infill_st_oct_rnd = 1
; support_Z_max_mm = 1e+20
; sheathe_Z_max_mm = 0
; inset_surface_xy_mm = 0
; seam_jitter_degrees = 0
; seam_depth_scaler = 1
; seam_gap_scaler = 1
; seam_angle_degrees = 45
; Speed vs Quality = 0.91
; Perimeter Speed = 11.80
; Solid Speed = 11.80
; Sparse Speed = 11.80
;
; *** G-code Prefix ***
;
%
;
; *** Main G-code ***
;
; BEGIN_LAYER_OBJECT z=0.50
;
; *** Warming Extruder 1 to 255 C ***


;
;
; *** Slowing to match Min Layer Time (speed multiplier is 0.019016) ***
; fan on
M106
;
; 'Perimeter Path', 0.0 [feed mm/s], 0.2 [head mm/s]
G1 X0.03 Y0.23 Z0.75 E0 F30000
G1 X0.03 Y0.23 Z0.5 E0 F210
G1 E1.25 F900
G1 X-0.05 Y0.23 E0.003 F11.4
G1 X-0.23 Y0.23 E0.0063
G1 X-0.23 Y-0.23 E0.0162
G1 X0.23 Y-0.23 E0.0163
G1 X0.23 Y-0.05 E0.0062
G1 X0.23 Y0.03 E0.0031
;
; Post-layer lift
G1 X0.23 Y0.03 Z1 E0 F210
; END_LAYER_OBJECT z=0.50
; BEGIN_LAYER_OBJECT z=0.75
;
; *** Warming Extruder 1 to 250 C ***


;
;
; *** Slowing to match Min Layer Time (speed multiplier is 0.017100) ***
;
; 'Perimeter Path', 0.0 [feed mm/s], 0.2 [head mm/s]
G1 X0.03 Y0.23 Z1 E0 F30000
G1 X0.03 Y0.23 Z0.75 E0 F210
G1 X-0.05 Y0.23 E0.0015 F12.1
G1 X-0.23 Y0.23 E0.0032
G1 X-0.23 Y-0.23 E0.0081
G1 X0.23 Y-0.23 E0.0081
G1 X0.23 Y-0.05 E0.0031
G1 X0.23 Y0.03 E0.0015
;
; Post-layer lift
G1 X0.23 Y0.03 Z1.25 E0 F210
; END_LAYER_OBJECT z=0.75
; BEGIN_LAYER_OBJECT z=1.00
; *** Slowing to match Min Layer Time (speed multiplier is 0.017100) ***
;
; 'Perimeter Path', 0.0 [feed mm/s], 0.2 [head mm/s]
G1 X0.03 Y0.23 Z1.25 E0 F30000
G1 X0.03 Y0.23 Z1 E0 F210
G1 X-0.05 Y0.23 E0.0015 F12.1
G1 X-0.23 Y0.23 E0.0032
G1 X-0.23 Y-0.23 E0.0081
G1 X0.23 Y-0.23 E0.0081
G1 X0.23 Y-0.05 E0.0031
G1 X0.23 Y0.03 E0.0015
;
; Post-layer lift
G1 X0.23 Y0.03 Z1.5 E0 F210
; END_LAYER_OBJECT z=1.00
; BEGIN_LAYER_OBJECT z=1.25
; *** Slowing to match Min Layer Time (speed multiplier is 0.056208) ***
;
; 'Perimeter Path', 0.0 [feed mm/s], 0.7 [head mm/s]
G1 X0.03 Y0.23 Z1.5 E0 F30000
G1 X0.03 Y0.23 Z1.25 E0 F210
G1 X-0.05 Y0.23 E0.0015 F39.8
G1 X-0.23 Y0.23 E0.0032
G1 X-0.23 Y-0.23 E0.0081
G1 X0.23 Y-0.23 E0.0081
G1 X0.23 Y-0.05 E0.0031
G1 X0.23 Y0.03 E0.0015
;
; 'Destring/Wipe/Jump Path', 0.0 [feed mm/s], 0.7 [head mm/s]
G1 X0.23 Y0.03 E0 F30000
G1 E-1.25 F900
G1 X0.23 Y0.03 E0 F39.8
G1 X0.03 Y0.23 E0
G1 X-0.05 Y0.23 E0
G1 X-0.23 Y0.23 E0
G1 X-0.23 Y-0.23 E0
G1 X0.23 Y-0.23 E0
G1 X0.23 Y-0.05 E0
G1 X0.23 Y0.03 E0
;
; Post-layer lift
G1 X0.23 Y0.03 Z2 E0 F210
; END_LAYER_OBJECT z=1.25
;
; *** Cooling Extruder 1 to 0 C ***


;
; fan off
M107
; *** G-code Postfix ***
;
%
;
;
;
; Estimated Build Time:   0.68 minutes
; Estimated Build Volume: 0.001 cm^3
; Estimated Build Cost:   $0.00
;
; *** Extrusion Time Breakdown ***
; * estimated time in [s]
; * before possibly slowing down for 'cool'
; * not including Z-travel
;	+-------------+-------------+-------------+-------------+----------------------------+
;	| Extruder #1 | Extruder #2 | Extruder #3 | Extruder #4 | Path Type                  |
;	+-------------+-------------+-------------+-------------+----------------------------+
;	| 0           | 0           | 0           | 0           | Jump Path                  |
;	| 0           | 0           | 0           | 0           | Pillar Path                |
;	| 0           | 0           | 0           | 0           | Raft Path                  |
;	| 0           > 0           > 0           > 0           > Support Interface Path     |
;	| 0           | 0           | 0           | 0           | Support (may Stack) Path   |
;	| 0.632846    | 0           | 0           | 0           | Perimeter Path             |
;	| 0           | 0           | 0           | 0           | Loop Path                  |
;	| 0           > 0           > 0           > 0           > Solid Path                 |
;	| 0           | 0           | 0           | 0           | Sparse Infill Path         |
;	| 0           | 0           | 0           | 0           | Stacked Sparse Infill Path |
;	| 0.351967    | 0           | 0           | 0           | Destring/Wipe/Jump Path    |
;	| 0           > 0           > 0           > 0           > Crown Path                 |
;	| 0           | 0           | 0           | 0           | Prime Pillar Path          |
;	| 0           | 0           | 0           | 0           | Skirt Path                 |
;	| 0           | 0           | 0           | 0           | Pause Point                |
;	| 0           > 0           > 0           > 0           > Extruder Warm-Up           |
;	+-------------+-------------+-------------+-------------+----------------------------+
; Total estimated (pre-cool) minutes: 0.02
