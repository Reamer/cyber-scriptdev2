DELETE FROM script_texts WHERE entry BETWEEN -1000634 AND -1000629;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1000629,'<cough> <cough> Damsel in distress over here!',0,0,0,0,'isla starmane - SAY_ISLA_PERIODIC_1'),
(-1000630,'Hello? Help?',0,0,0,0,'isla starmane - SAY_ISLA_PERIODIC_2'),
(-1000631,'Don''t leave me in here! Cause if you do I will find you!',0,0,0,0,'isla starmane - SAY_ISLA_PERIODIC_3'),
(-1000632,'Ok, let''s get out of here!',0,0,0,0,'isla starmane - SAY_ISLA_START'),
(-1000633,'You sure you''re ready? Take a moment.',0,0,0,0,'isla starmane - SAY_ISLA_WAITING'),
(-1000634,'Alright, let''s do this!',0,0,0,0,'isla starmane - SAY_ISLA_LEAVE_BUILDING');

DELETE FROM script_waypoint WHERE entry = 18760;
INSERT INTO script_waypoint (entry, pointid, location_x, location_y, location_z, waittime, point_comment) VALUES
(18760, 01, -2267.07, 3091.46, 13.8271, 0, ''),
(18760, 02, -2270.92, 3094.19, 13.8271, 0, ''),
(18760, 03, -2279.08, 3100.04, 13.8271, 0, ''),
(18760, 04, -2290.05, 3105.07, 13.8271, 0, ''),
(18760, 05, -2297.64, 3112.32, 13.8271, 0, ''),
(18760, 06, -2303.89, 3118.22, 13.8231, 10000, 'building exit'),
(18760, 07, -2307.77, 3123.47, 13.7257, 0, ''),
(18760, 08, -2310.67, 3126.2,  12.5841, 0, ''),
(18760, 09, -2311.48, 3126.98, 12.2769, 0, ''),
(18760, 10, -2316.91, 3132.13, 11.9261, 0, ''),
(18760, 11, -2320.43, 3135.54, 11.7436, 0, ''),
(18760, 12, -2327.38, 3139.36, 10.9431, 0, ''),
(18760, 13, -2332.02, 3142.05, 9.81277, 0, ''),
(18760, 14, -2338.21, 3145.32, 9.31001, 0, ''),
(18760, 15, -2343.1,  3148.91, 8.84879, 0, ''),
(18760, 16, -2347.76, 3153.15, 7.71049, 0, ''),
(18760, 17, -2351.04, 3156.12, 6.66476, 0, ''),
(18760, 18, -2355.15, 3163.18, 5.11997, 0, ''),
(18760, 19, -2359.01, 3169.83, 3.64343, 0, ''),
(18760, 20, -2364.85, 3176.81, 2.32802, 0, ''),
(18760, 21, -2368.77, 3181.69, 1.53285, 0, ''),
(18760, 22, -2371.76, 3185.11, 0.979932, 0, ''),
(18760, 23, -2371.85, 3191.89, -0.293048, 0, ''),
(18760, 24, -2370.99, 3199.6, -1.10504, 0, 'turn left 1'),
(18760, 25, -2376.24, 3205.54, -1.04152, 0, ''),
(18760, 26, -2380.99, 3211.61, -1.16891, 0, ''),
(18760, 27, -2384.04, 3218.4, -1.15279, 0, ''),
(18760, 28, -2385.41, 3226.22, -1.23403, 0, ''),
(18760, 29, -2386.02, 3233.89, -1.31723, 0, ''),
(18760, 30, -2384.7, 3239.82, -1.51903, 0, ''),
(18760, 31, -2382.98, 3247.94, -1.39163, 0, ''),
(18760, 32, -2379.68, 3254.22, -1.25927, 0, ''),
(18760, 33, -2375.27, 3259.69, -1.22925, 0, ''),
(18760, 34, -2369.62, 3264.55, -1.1879, 0, ''),
(18760, 35, -2364.01, 3268.32, -1.48348, 0, ''),
(18760, 36, -2356.61, 3272.31, -1.5505, 0, ''),
(18760, 37, -2352.3, 3274.63, -1.35693, 0, ''),
(18760, 38, -2348.54, 3278.04, -1.04161, 0, 'turn left 2'),
(18760, 39, -2347.56, 3282.41, -0.75979, 0, ''),
(18760, 40, -2348.29, 3288.91, -0.63215, 0, ''),
(18760, 41, -2349.68, 3298.84, -1.07864, 0, ''),
(18760, 42, -2351.15, 3308.52, -1.38864, 0, ''),
(18760, 43, -2352.2, 3317.14, -1.59873, 0, ''),
(18760, 44, -2351.59, 3325.51, -1.92624, 0, ''),
(18760, 45, -2350.88, 3333.38, -2.32506, 0, ''),
(18760, 46, -2350.05, 3342.68, -2.51886, 0, ''),
(18760, 47, -2350.12, 3347.32, -2.57528, 0, ''),
(18760, 48, -2348.72, 3353.7, -2.72689, 0, ''),
(18760, 49, -2346.53, 3360.85, -2.9756, 0, ''),
(18760, 50, -2344.83, 3365.46, -3.3311, 0, ''),
(18760, 51, -2342.68, 3368.91, -3.74526, 0, ''),
(18760, 52, -2340.25, 3371.44, -4.10499, 0, ''),
(18760, 53, -2337.4, 3373.47, -4.44362, 0, ''),
(18760, 54, -2332.68, 3376.02, -5.19648, 0, ''),
(18760, 55, -2326.69, 3379.64, -6.24757, 0, ''),
(18760, 56, -2321.2, 3383.98, -7.28247, 0, ''),
(18760, 57, -2317.81, 3387.78, -8.40093, 0, ''),
(18760, 58, -2315.3, 3392.47, -9.63431, 0, ''),
(18760, 59, -2314.69, 3396.6, -10.2031, 0, ''),
(18760, 60, -2315.48, 3402.35, -10.8211, 0, 'gate'),
(18760, 61, -2317.55, 3409.27, -11.3309, 5000, 'Firewing point exit'),
(18760, 62, -2320.69, 3412.99, -11.5207, 0, ''),
(18760, 63, -2326.88, 3417.89, -11.6105, 0, ''),
(18760, 64, -2332.73, 3421.74, -11.5659, 0, ''),
(18760, 65, -2337.23, 3424.89, -11.496, 0, ''),
(18760, 66, -2339.57, 3429.17, -11.3782, 0, ''),
(18760, 67, -2341.66, 3435.86, -11.3746, 5000, 'Wave and transform'),
(18760, 68, -2342.15, 3443.94, -11.2562, 2000, 'final destination');
