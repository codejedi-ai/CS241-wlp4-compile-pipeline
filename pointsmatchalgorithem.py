# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

def generate_fill_commands():
    directions = ['north', 'east', 'south', 'west']  # Cardinal directions

    for direction in directions:
        fill_command = f"/fill 28 66 158 ~ ~ ~ minecraft:diorite_stairs[facing={direction}] replace snowrealmagic:stairs[facing={direction}]"
        print(fill_command)  # You can modify this line to execute the command in your Minecraft environment
import os
import re

import os

def get_file_names(folder_path):
    """
    Retrieves a list of all file names in a folder.

    Args:
        folder_path (string): Path to the folder.

    Returns:
        list: List of file names in the folder.
    """
    file_names = []

    for filename in os.listdir(folder_path):
        if os.path.isfile(os.path.join(folder_path, filename)):
            file_names.append(filename)

    return file_names


def extract_coordinates_from_filenames(folder_path):
    """
    Extracts (x, y) tuples from filenames in a folder.

    Args:
        folder_path (string): Path to the folder containing the filenames.

    Returns:
        set: Set of (x, y) tuples extracted from the filenames.
    """
    coordinate_set = set()

    # Regular expression pattern to extract (x, y) values from filenames
    pattern = r"r\.(\d+)\.(\d+)\.mca"

    for filename in os.listdir(folder_path):
        # Match the pattern against the filename
        match = re.match(pattern, filename)
        if match:
            x = int(match.group(1))
            y = int(match.group(2))
            coordinate_set.add((x, y))

    return coordinate_set







def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.
    # Generate and execute the fill commands
    generate_fill_commands()

import os
import re
import numpy as np

def mca_folder_to_points(folder_path):
    # folder_path = "E:/Minecraft/Aug_12_2021/0_First_Majore_Base_Project/region"
    # Example usage
    file_names = get_file_names(folder_path)
    matching_strings = [filename for filename in file_names if re.match( r"r\.(-?\d+)\.(-?\d+)\.mca", filename)]
    points = [(int(i.split(".")[1]),int(i.split(".")[2])) for i in matching_strings]
    return points

import numpy as np

def get_bounding_box(points):
    """
    Calculates the bounding box of a set of points.

    Args:
        points (list): List of (x, y) coordinate pairs representing points.

    Returns:
        ((min_x, min_y), (max_x, max_y)): Bounding box coordinates as a pair of tuples.
    """
    points = np.array(points)
    min_x, min_y = np.min(points, axis=0)
    max_x, max_y = np.max(points, axis=0)
    return (min_x, min_y), (max_x, max_y)



# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    # print_hi('PyCharm')
    generate_fill_commands()
    print(mca_folder_to_points("E:/Minecraft/Aug_12_2021/0_First_Majore_Base_Project/region"))
    # Example usage
    # Example usage
    root_folder = r"E:\Minecraft\Aug_12_2021"
    folders_under = ([root for root in os.listdir(root_folder)])
    # append the root folder to each element of folders_under
    folders_under = [os.path.join(root_folder, root) for root in folders_under]
    folders_under = [os.path.join(root, "region") for root in folders_under]
    # iterate through each folder
    jigsaw = [mca_folder_to_points(folder) for folder in folders_under if os.path.isdir(folder)]
    print(jigsaw)
    # now shift each element in jigsaw by the x and y coordinates of the first element
    # such that no two element have the same x and y coordinates
    # first find the minimum x and y coordinates
    print(folders_under)
    # Example usage
    bounding_box = [get_bounding_box(points) for points in jigsaw]
    print(f"Bounding box: {bounding_box}")
    for d in zip(folders_under, jigsaw, bounding_box):
        print(d)
