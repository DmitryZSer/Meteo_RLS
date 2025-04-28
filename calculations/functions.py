import numpy as np
import re

def read_pws_file(filename):
    with open(filename, 'r') as f:
        title = f.readline().strip()          # Title/description
        var_name = f.readline().strip()       # Variable name
        ix, iy, iz = map(int, f.readline().split())  # Grid dimensions (x, y, z)
         
        time_line = list(map(float, f.readline().split()))
        sim_time = time_line[0]# simtime - time of the simulation in seconds
        xstart, ystart, zstart = time_line[1], time_line[2], time_line[3] #origin coordinates
        
        # (dx, dy, dz)
        dx, dy, dz = map(float, f.readline().split())
        
        data_flat = []
        for line in f:
            matches = re.findall(r'-?\d+\.\d+E[+-]\d+', line)
            data_flat.extend(list(map(float, matches)))
        
        # 3D array 
        data_3d = np.array(data_flat).reshape((ix, iy, iz), order='F')
    
    return {
        'title': title,
        'variable': var_name,
        'dimensions': (ix, iy, iz),
        'sim_time_seconds': sim_time,
        'origin_m': (xstart, ystart, zstart),
        'resolution_m': (dx, dy, dz),
        'data': data_3d  # 3D array [x][y][z]
    }
    
def print_pws_files_data(file_list):
    for file_path in file_list:
        print(f"Processing file: {file_path}")
        try:
            data = read_pws_file(file_path)
            
            print("Metadata:")
            print(f"  Title: {data['title']}")
            print(f"  Variable: {data['variable']}")
            print(f"  Dimensions (x, y, z): {data['dimensions']}")
            print(f"  Simulation Time (seconds): {data['sim_time_seconds']}")
            print(f"  Origin (x, y, z) in meters: {data['origin_m']}")
            print(f"  Resolution (dx, dy, dz) in meters: {data['resolution_m']}")
            
            print("\nSample Data:")
            print(data['data'][:, :, :], "end\n") 
            print(data['data'][:, :, :], "end\n") 
            print(f"Data max value:\n{np.max(data['data'])}")
            print(f"Data min value:\n{np.min(data['data'])}")

            
            print("-" * 80)
        except Exception as e:
            print(f"Error processing file {file_path}: {e}")
            