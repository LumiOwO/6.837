@echo off

..\Debug\6.837.exe -input scene6_01_sphere.txt -output output6_01a.tga -size 200 200 -stats
..\Debug\6.837.exe -input scene6_01_sphere.txt -output output6_01b.tga -size 200 200 -grid 10 10 10 -stats
..\Debug\6.837.exe -input scene6_01_sphere.txt -output output6_01c.tga -size 200 200 -grid 10 10 10 -visualize_grid
 
..\Debug\6.837.exe -input scene6_02_sphere_triangles.txt -output output6_02a.tga -size 200 200 -stats
..\Debug\6.837.exe -input scene6_02_sphere_triangles.txt -output output6_02b.tga -size 200 200 -grid 10 10 10 -stats 
..\Debug\6.837.exe -input scene6_02_sphere_triangles.txt -output output6_02c.tga -size 200 200 -stats -shadows
..\Debug\6.837.exe -input scene6_02_sphere_triangles.txt -output output6_02d.tga -size 200 200 -grid 10 10 10 -stats -shadows 
..\Debug\6.837.exe -input scene6_02_sphere_triangles.txt -output output6_02e.tga -size 200 200 -grid 10 10 10 -visualize_grid
  
..\Debug\6.837.exe -input scene6_03_sphere_plane.txt -output output6_03a.tga -size 200 200 -stats
..\Debug\6.837.exe -input scene6_03_sphere_plane.txt -output output6_03b.tga -size 200 200 -grid 10 10 10 -stats
..\Debug\6.837.exe -input scene6_03_sphere_plane.txt -output output6_03c.tga -size 200 200 -stats -shadows
..\Debug\6.837.exe -input scene6_03_sphere_plane.txt -output output6_03d.tga -size 200 200 -grid 10 10 10 -stats -shadows
..\Debug\6.837.exe -input scene6_03_sphere_plane.txt -output output6_03e.tga -size 200 200 -grid 10 10 10 -visualize_grid
  
rem ..\Debug\6.837.exe -input scene6_04_bunny_mesh_200.txt -output output6_04a.tga -size 200 200 -stats
rem ..\Debug\6.837.exe -input scene6_04_bunny_mesh_200.txt -output output6_04b.tga -size 200 200 -grid 10 10 7 -stats
rem ..\Debug\6.837.exe -input scene6_04_bunny_mesh_200.txt -output output6_04c.tga -size 200 200 -stats -shadows
rem ..\Debug\6.837.exe -input scene6_04_bunny_mesh_200.txt -output output6_04d.tga -size 200 200 -grid 10 10 7 -stats -shadows
rem ..\Debug\6.837.exe -input scene6_04_bunny_mesh_200.txt -output output6_04e.tga -size 200 200 -grid 10 10 7 -visualize_grid
rem ..\Debug\6.837.exe -input scene6_05_bunny_mesh_1k.txt  -output output6_05.tga -size 200 200 -grid 15 15 12 -stats -shadows
rem ..\Debug\6.837.exe -input scene6_06_bunny_mesh_5k.txt  -output output6_06.tga -size 200 200 -grid 20 20 15 -stats -shadows
rem ..\Debug\6.837.exe -input scene6_07_bunny_mesh_40k.txt -output output6_07.tga -size 200 200 -grid 40 40 33 -stats -shadows
  
  
..\Debug\6.837.exe -input scene6_08_scale_translate.txt -size 200 200 -output output6_08a.tga
..\Debug\6.837.exe -input scene6_08_scale_translate.txt -size 200 200 -output output6_08b.tga -grid 15 15 15
..\Debug\6.837.exe -input scene6_08_scale_translate.txt -size 200 200 -output output6_08c.tga -grid 15 15 15 -visualize_grid
 
..\Debug\6.837.exe -input scene6_09_rotated_triangles.txt -size 200 200 -output output6_09a.tga
..\Debug\6.837.exe -input scene6_09_rotated_triangles.txt -size 200 200 -output output6_09b.tga -grid 15 15 9 
..\Debug\6.837.exe -input scene6_09_rotated_triangles.txt -size 200 200 -output output6_09c.tga -grid 15 15 9 -visualize_grid
 
..\Debug\6.837.exe -input scene6_10_nested_transformations.txt -size 200 200 -output output6_10a.tga
..\Debug\6.837.exe -input scene6_10_nested_transformations.txt -size 200 200 -output output6_10b.tga -grid 30 30 30
..\Debug\6.837.exe -input scene6_10_nested_transformations.txt -size 200 200 -output output6_10c.tga -grid 30 30 30 -visualize_grid
 
..\Debug\6.837.exe -input scene6_11_mirrored_floor.txt -size 200 200 -output output6_11a.tga -shadows -bounces 1 -weight 0.01 -stats
..\Debug\6.837.exe -input scene6_11_mirrored_floor.txt -size 200 200 -output output6_11b.tga -shadows -bounces 1 -weight 0.01 -grid 40 10 40 -stats
..\Debug\6.837.exe -input scene6_11_mirrored_floor.txt -size 200 200 -output output6_11c.tga -grid 40 10 40 -visualize_grid
 
..\Debug\6.837.exe -input scene6_12_faceted_gem.txt -size 200 200 -output output6_12a.tga -shadows -shade_back -bounces 5 -weight 0.01 -stats
..\Debug\6.837.exe -input scene6_12_faceted_gem.txt -size 200 200 -output output6_12b.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -stats
..\Debug\6.837.exe -input scene6_12_faceted_gem.txt -size 200 200 -output output6_12c.tga -grid 20 20 20 -visualize_grid
 
..\Debug\6.837.exe -input scene6_13_checkerboard.txt -size 200 200 -output output6_13.tga -shadows 
..\Debug\6.837.exe -input scene6_14_glass_sphere.txt -size 200 200 -output output6_14.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20
 
rem ..\Debug\6.837.exe -input scene6_15_marble_cubes.txt -size 300 300 -output output6_15.tga 
rem ..\Debug\6.837.exe -input scene6_16_wood_cubes.txt -size 300 300 -output output6_16.tga 
 
rem ..\Debug\6.837.exe -input scene6_17_marble_vase.txt -size 300 300 -output output6_17a.tga -grid 15 30 15 -bounces 1 -shadows
rem ..\Debug\6.837.exe -input scene6_17_marble_vase.txt -size 300 300 -output output6_17b.tga -grid 15 30 15 -visualize_grid
 
rem ..\Debug\6.837.exe -input scene6_18_6.837_logo.txt -size 400 200 -output output6_18a.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 80 30 3
rem ..\Debug\6.837.exe -input scene6_18_6.837_logo.txt -size 400 200 -output output6_18b.tga -grid 80 30 3 -visualize_grid

pause