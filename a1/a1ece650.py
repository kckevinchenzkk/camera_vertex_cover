#!/usr/bin/env python3
import sys
import math
import re
# YOUR CODE GOES HERE
# Define Street object
class Street(object):
    def __init__(self, name, coordinate):
        self.name = name
        self.coordinate = coordinate
        self.intersection_list = []
    def get_name(self):
        return self.name
    def get_coordinate(self):
        return self.coordinate

def gg():
    global street_list
    global V,E,V_index
    coordinate_list = None
    exception_list = []
            
    # check if each street has intersection with the other street
    for i in range(len(street_list)):
        for j in range(i + 1, len(street_list)): 
            # check through each edge
            intersection_list = []
            intersection_index_list = []
            intersection_vetex_list = []
            #first_intersection = True
            for coordinate_index in range(1, len(street_list[i].coordinate)):
                coordinate_1 = street_list[i].coordinate[coordinate_index-1]
                coordinate_2 = street_list[i].coordinate[coordinate_index]
                coordinate_1 = coordinate_1[1:-1].split(",")
                coordinate_2 = coordinate_2[1:-1].split(",")
                # check intersection for another line by each edge
                for coordinate_index_2 in range(1, len(street_list[j].coordinate)):
                    coordinate_3 = street_list[j].coordinate[coordinate_index_2 - 1]
                    coordinate_4 = street_list[j].coordinate[coordinate_index_2] 
                    #print(coordinate_1, coordinate_2,coordinate_3, coordinate_4)
                    coordinate_3 = coordinate_3[1:-1].split(",")
                    coordinate_4 = coordinate_4[1:-1].split(",")
                    
                    coordinate_1 = [float(coordinate_1[0]), float(coordinate_1[1])]
                    coordinate_2 = [float(coordinate_2[0]), float(coordinate_2[1])]
                    coordinate_3 = [float(coordinate_3[0]), float(coordinate_3[1])]
                    coordinate_4 = [float(coordinate_4[0]), float(coordinate_4[1])]
                    coordinate_1 = ['{:.0f}'.format(coord) if coord.is_integer() else '{:.5f}'.format(coord)
                    for coord in coordinate_1]
                    coordinate_2 = ['{:.0f}'.format(coord) if coord.is_integer() else '{:.5f}'.format(coord)
                    for coord in coordinate_2]
                    coordinate_3 = ['{:.0f}'.format(coord) if coord.is_integer() else '{:.5f}'.format(coord)
                    for coord in coordinate_3]
                    coordinate_4 = ['{:.0f}'.format(coord) if coord.is_integer() else '{:.5f}'.format(coord)
                    for coord in coordinate_4]
                    
                    #print(coordinate_1, coordinate_2,coordinate_3, coordinate_4)
                    l1 = line(point(float(coordinate_1[0]),float(coordinate_1[1])), point(float(coordinate_2[0]), float(coordinate_2[1])))
                    l2 = line(point(float(coordinate_3[0]),float(coordinate_3[1])), point(float(coordinate_4[0]), float(coordinate_4[1])))
                    
                    if intersect(l1,l2):
                        #first_intersection = False
                        #print(coordinate_index - 1, coordinate_1, coordinate_3)
                        #print('Intersection of', l1, 'with', l2, 'is', intersect(l1, l2) )
                        
                        coordinate_1_Vindex = None
                        coordinate_2_Vindex = None
                        coordinate_3_Vindex = None
                        coordinate_4_Vindex = None
                        intersection_Vindex = None
                        if not coordinate_1 in V.values():
                            V[V_index] = coordinate_1
                            coordinate_1_Vindex = V_index
                            V_index += 1
                        else:
                            for idx, value in V.items():
                                if value == coordinate_1:
                                    coordinate_1_Vindex = idx
                                    break
                        #print(coordinate_1_Vindex)
                        if not coordinate_3 in V.values():
                            V[V_index] = coordinate_3
                            coordinate_3_Vindex = V_index
                            V_index += 1
                        else:
                            for idx, value in V.items():
                                if value == coordinate_3:
                                    coordinate_3_Vindex = idx
                                    break
                        intersection_list.append(intersect(l1,l2))
                        intersection_coordinate = [intersect(l1,l2).x, intersect(l1,l2).y]
                        intersection_coordinate = ['{:.0f}'.format(coord) if coord.is_integer() else '{:.5f}'.format(coord)
                        for coord in intersection_coordinate]

                        # if the intersection point is not in the V
                        if not intersection_coordinate in V.values():
                            V[V_index] = intersection_coordinate
                            intersection_Vindex = V_index
                            V_index += 1
                        else:
                            for idx, value in V.items():
                                if value == intersection_coordinate:
                                    intersection_Vindex = idx
                                    break
                        intersection_index_list.append(intersection_Vindex)
                        if not coordinate_2 in V.values():
                            V[V_index] = coordinate_2
                            coordinate_2_Vindex = V_index
                            V_index += 1
                        else:
                            for idx, value in V.items():
                                if value == coordinate_2:
                                    coordinate_2_Vindex = idx
                                    break

                        if not coordinate_4 in V.values():
                            V[V_index] = coordinate_4
                            coordinate_4_Vindex = V_index
                            V_index += 1
                        else:
                            for idx, value in V.items():
                                if value == coordinate_4:
                                    coordinate_4_Vindex = idx
                                    break
                        intersection_vetex_list.append((coordinate_1_Vindex,coordinate_2_Vindex,coordinate_3_Vindex,coordinate_4_Vindex))
                        if not intersection_coordinate in street_list[i].intersection_list:
                            street_list[i].intersection_list.append(intersection_coordinate)
                        if not intersection_coordinate in street_list[j].intersection_list:
                            street_list[j].intersection_list.append(intersection_coordinate)
                        #print(V)
            # if more than one intersection between two streets
            if len(intersection_list) > 1:
                duplicated_vetex_list = []
                seen_vetex = []
                for tup in intersection_vetex_list:
                    for element in tup:
                        if element in seen_vetex:
                            duplicated_vetex_list.append(element)
                        else:
                            seen_vetex.append(element)
                # for not repeated vetex
                for idx, tup in enumerate(intersection_vetex_list):
                    for element in tup:
                        if not element in duplicated_vetex_list:
                            if not (element, intersection_index_list[idx]) in E and not (intersection_index_list[idx], element) in E and not element == intersection_index_list[idx]:
                                E.append((element, intersection_index_list[idx]))
                # here deal with the repeated vetex
                for intersection_idx in intersection_index_list:
                    shortest_dist = float('inf')
                    shortest_pair = None
                    shortest_pair_inv = None
                    for vertex_idx in duplicated_vetex_list:
                        vertex_coor = V[vertex_idx]
                        intersection_coor = V[intersection_idx]
                        dist = math.sqrt((float(vertex_coor[0]) - float(intersection_coor[0]))**2 + (float(vertex_coor[1]) - float(intersection_coor[1]))**2)
                        if dist < shortest_dist:
                            exception_list.append(shortest_pair)
                            exception_list.append(shortest_pair_inv)
                            shortest_dist = dist
                            shortest_pair = (vertex_idx, intersection_idx)
                            shortest_pair_inv = (intersection_idx, vertex_idx)
                        else:
                            exception_list.append((vertex_idx, intersection_idx))
                            exception_list.append(((intersection_idx, vertex_idx)))
                    if shortest_pair != None:
                        if not shortest_pair in E and not shortest_pair_inv in E and not shortest_pair[0] == shortest_pair[1]:
                            E.append(shortest_pair)
                for index in range(len(intersection_index_list)):
                    for index_1 in range(index+1, len(intersection_index_list)):
                        if not (intersection_index_list[index],intersection_index_list[index_1]) in E and not (intersection_index_list[index_1],intersection_index_list[index]) in E and not intersection_index_list[index] == intersection_index_list[index_1]:
                            E.append((intersection_index_list[index],intersection_index_list[index_1]))
                # only one intersection between two streets
            elif len(intersection_list) == 1:
                for idx, tup in enumerate(intersection_vetex_list):
                        for element in tup:
                            if not (element, intersection_index_list[0]) in E and not (intersection_index_list[0], element) in E and not(intersection_index_list[0], element) in exception_list and not element == intersection_index_list[0]:
                                E.append((element, intersection_index_list[0]))
            check_single_intersection()
            # remove duplicated connection:
            #pop_exception_E()
            pop_idx = []
            for idx, value in enumerate(E):
                if value in exception_list:
                    pop_idx.append(idx)
            E = [E[i] for i in range(len(E)) if i not in pop_idx]
    rewrite_E()

def rewrite_E():
    global V,E, street_list
    new_E = []
    vertex_list_formatted = []
    intersect_list_formatted = []
    
    # find all vertex and intersection
    for idx_street, street in enumerate(street_list):
        vertex_list_formatted = []
        intersect_list_formatted = []
        vertex_list = street.coordinate
        for vertex_coor in vertex_list:
            vertex_coor = vertex_coor[1:-1].split(",")
            vertex_list_formatted.append(vertex_coor)
        vertex_list_formatted = [(float(item[0]), float(item[1])) for item in vertex_list_formatted]

        intersect_list = street.intersection_list
        for intersect_coor in intersect_list:
            intersect_list_formatted.append((float(intersect_coor[0]),float(intersect_coor[1])))
        intersect_list_formatted= [(float(item[0]), float(item[1])) for item in intersect_list_formatted]
    # calculate dist between vertex and its cloest intersection
        intersect_list_occurance = [0] * len(intersect_list_formatted)
        shortest_pair_list = []

        vertex_between_inter_list = []
        for idx_vertex, vertex_coor in enumerate(vertex_list_formatted):
            if len(vertex_list_formatted) > 2:
                checklist = [0]* len(vertex_list_formatted)
                for idx, coor in enumerate(vertex_list_formatted):
                    if find_key_by_value(V, coor):
                        checklist[idx] = 1
                indices_of_1 = [i for i, x in enumerate(checklist) if x == 1]
                if len(indices_of_1) > 1:
                    vertex_idx_between_inter_list = indices_of_1[1:-1]
                else:
                    vertex_idx_between_inter_list = []
                for idx in vertex_idx_between_inter_list:
                    vertex_between_inter_list.append(vertex_list_formatted[idx])
            shortest_dist = float('inf')
            shortest_pair = None
            shortest_pair_intersection_idx = None
            identical = False
            vertex_idxV = find_key_by_value(V, vertex_coor)
            if not vertex_idxV:
                continue
            for idx_intersection, intersection_coor in enumerate(intersect_list_formatted):
                intersection_idxV = find_key_by_value(V, intersection_coor)
                dist = math.sqrt((float(vertex_coor[0]) - float(intersection_coor[0]))**2 + (float(vertex_coor[1]) - float(intersection_coor[1]))**2)
                #print(dist, shortest_dist, dist < shortest_dist)
                if dist < shortest_dist:
                    # if vertex_idxV == intersection_idxV:
                    #     continue
                    shortest_dist = dist
                    shortest_pair = (vertex_idxV, intersection_idxV)
                    shortest_pair_intersection_idx = idx_intersection
                    if vertex_idxV == intersection_idxV:
                        identical = True
                    else:
                        identical = False
                    #shortest_pair_inv = (intersection_idxV, vertex_idxV)
            if shortest_pair_intersection_idx !=None:
                intersect_list_occurance[shortest_pair_intersection_idx] += 1
            if not identical:
                shortest_pair_list.append(shortest_pair)
        # find the intersection to doesnt occur 2 times
        intersection_need_connect = []
        for idx, value in enumerate(intersect_list_occurance):
            if value < 2:
                intersection_need_connect.append(intersect_list_formatted[idx])
        finish = False
        count = 0
        intersection_remain = []
        while not finish and count < 1000:
            count += 1
            for idx in range(0, len(intersection_need_connect)):
                if intersect_list_occurance[idx] > 1:
                    continue
                shortest_dist = float('inf')
                shortest_pair = None
                shortest_pair_intersection_idx = None
                identical = False
                shortest_idx = None
                shortest_idx1 = None
                for idx_1 in range(idx+1, len(intersection_need_connect)):
                    if intersect_list_occurance[idx_1] > 1:
                        continue
                    coor0 = intersection_need_connect[idx]
                    coor1 = intersection_need_connect[idx_1]
                    coor0_idxV = find_key_by_value(V, coor0)
                    coor1_idxV = find_key_by_value(V, coor1)
                    dist = math.sqrt((float(coor0[0]) - float(coor1[0]))**2 + (float(coor0[1]) - float(coor1[1]))**2)
                    if dist < shortest_dist:
                        shortest_dist = dist
                        shortest_pair = (coor0_idxV, coor1_idxV)
                        shortest_idx = idx
                        shortest_idx1 = idx_1
                        if coor0_idxV == coor1_idxV:
                            identical = True
                        else:
                            identical = False
                if not identical and shortest_pair:
                    shortest_pair_list.append(shortest_pair)
                    if shortest_idx != None:
                        intersect_list_occurance[shortest_idx] += 1
                        if shortest_idx1 !=None:
                            intersect_list_occurance[shortest_idx1] += 1
            intersection_remain = []
            for idx, value in enumerate(intersect_list_occurance):
                if value < 2:
                    intersection_remain.append(intersect_list_formatted[idx])
            for value in intersect_list_occurance:
                if value < 2:
                    finish = False
                    break
                else:
                    finish = True
            if count == 1000:
                finish = True

        # for remaining intersection
        if len(intersection_remain) > 0:
            for intersection_coor in intersection_remain:
                shortest_dist = float('inf')
                shortest_pair = None
                shortest_pair_intersection_idx = None
                identical = False
                for vertex_coor in vertex_between_inter_list:
                    vertex_idxV = find_key_by_value(V, vertex_coor)
                    if not vertex_idxV:
                        continue
            # for idx_intersection, intersection_coor in enumerate(intersect_list_formatted):
                    intersection_idxV = find_key_by_value(V, intersection_coor)
                    dist = math.sqrt((float(vertex_coor[0]) - float(intersection_coor[0]))**2 + (float(vertex_coor[1]) - float(intersection_coor[1]))**2)
                    #print(dist, shortest_dist, dist < shortest_dist)
                    if dist < shortest_dist:
                        # if vertex_idxV == intersection_idxV:
                        #     continue
                        shortest_dist = dist
                        shortest_pair = (vertex_idxV, intersection_idxV)
                        #shortest_pair_intersection_idx = idx_intersection
                        if vertex_idxV == intersection_idxV:
                            identical = True
                        else:
                            identical = False
                        #shortest_pair_inv = (intersection_idxV, vertex_idxV)
                #intersect_list_occurance[shortest_pair_intersection_idx] += 1
                if not identical:
                    shortest_pair_list.append(shortest_pair)

        for pair in shortest_pair_list:
            new_E.append(pair)
        #print("end")
    E = new_E


def find_key_by_value(dictionary, value_to_find):
    # Iterate through the dictionary items
    for key, value in dictionary.items():
        # Check if the current value matches the value to find
        value = (float(value[0]),float(value[1]))
        #value= [(float(item[0]), float(item[1])) for item in value]
        #print(type(value), type(value_to_find), value, value_to_find, value==value_to_find)
        if value == value_to_find:
            return key  # Return the key if a match is found
    # Return None if the value is not found in the dictionary
    return None

# Line Parser
def parseLine(line_input):
    global street_list
    global V,E,V_index
    """Parse an input line and retrun command and argument 
       
       Throws an exception on error.
    """
    line_input = re.sub(r'\(\s*(.*?)\s*\)', lambda x: f'({"".join(x.group(1).split())})', line_input)
    line_ele = line_input.strip().split()
    cmd = line_ele[0]
    
    ###debugging
    #print(line_ele)

    coordinate_list = None
    # Raise error if only have one input but it is not gg
    if len(line_ele) == 1:
        if cmd == "gg":
            gg()
            # continue here just adding all edge before connecting them
            # if two line intersect -> link then if these two lines intersect again -> orginally like 1->3 -> 2 if added intersection bewtween 
            # 1 and 3 and then just modify by 1->4->3->2 same as the other one
            
            output_V()
            print(output_E())
            # for element in street_list:
            #     print(element.coordinate)
            #     print(element.intersection_list)
        else:
            print('Error: unknown command: ' + cmd, file=sys.stderr)
    else:
        # Get street name
        line_nameNcoor = line_ele[1:]
        street_name = line_nameNcoor[0]
        vetex_list = []
        if not '"' in line_nameNcoor[0]:
            print("Error: Wrong input format",file=sys.stderr)
            return
        for i in range(1,len(line_nameNcoor)):
            if street_name[-1] == '"':
                vetex_list = line_nameNcoor[i:]
                break
            street_name += line_nameNcoor[i]
            if '"' in line_nameNcoor[i]:
                vetex_list = line_nameNcoor[i+1:]
                break
            if "(" in line_nameNcoor[i]:
                print("Error: Wrong format for street name", file=sys.stderr)
                return
        if len(vetex_list) == 0 and cmd != "rm":
            print("Error: Missing coordinates for street: " + street_name, file=sys.stderr)
            return
        #Implemented different functions
        if cmd == 'add':
            pattern = r'^\(\s*-?\s*\d+\s*,\s*-?\s*\d+\s*\)$'
            for item in vetex_list:
                if not re.match(pattern, item):
                    print("Error: Wrong input coordinates format for: " + line_input, file=sys.stderr)
                    return
            for i in range(len(street_list)):
                if street_name == street_list[i].name:
                    print("Error: Street already existed and cannot be added: " + street_name, file=sys.stderr)
                    return
            if len(vetex_list) < 2:
                print("Error: Too few coordinates for: " + street_name, file=sys.stderr)
                return
            street_list.append(Street(street_name, vetex_list))
        elif cmd == 'rm':
            original_coor_list = []
            original_coor_Vindex = []
            original_coor_Eindex = []
            original_intersection_list = []
            intersection_index_list = []
            street_index = None
            existance = False
            for i in range(len(street_list)):
                if street_name == street_list[i].name:
                    street_index = i
                    existance = True
                    if len(V) != 0:
                        original_coor_list = street_list[i].coordinate
                        original_intersection_list = street_list[i].intersection_list
                        intersection_occurances_list = [] 
                        for coor in original_coor_list:
                            coor = coor[1:-1].split(",")
                            coor = [str(coor[0]), str(coor[1])]
                            for idx, value in V.items():
                                if value == coor:
                                    original_coor_Vindex.append(idx)
            if not existance:
                print("Error: Street doesn't exist and cannot be removed: " + street_name, file=sys.stderr)
                return
            else:
                intersection_occurances_list = [0] * len(original_intersection_list)
                for i in range(len(street_list)):
                    if i == street_index:
                        continue
                    else:
                        for coor in street_list[i].intersection_list:
                            for idx,intersection_coor in enumerate(original_intersection_list):
                                if intersection_coor == coor:
                                    intersection_occurances_list[idx] += 1
                                    
                #print(original_intersection_list)
                #print(intersection_occurances_list)

                for idx, value in enumerate(intersection_occurances_list):
                    if value < 2:
                        for idx_V, coor in V.items():
                            if original_intersection_list[idx] == coor:
                                original_coor_Vindex.append(idx_V)
                                intersection_index_list.append(idx_V)
            

                ### missing intersection!!!!!
                for idx in original_coor_Vindex:
                    for i in range(len(street_list)):
                        if i == street_index:
                            continue
                        else:
                            for street_list_index, element in enumerate(street_list[i].intersection_list):
                                # coor = (V[idx][0], V[idx][1])
                                # element = ()
                                if V[idx] == element:
                                    street_list[i].intersection_list.pop(street_list_index)
                    V.pop(idx)
                # double check for V
                V_pop_index = []
                for idx_V, coor in V.items():
                    for i in range(len(street_list)):
                        for street_list_coordinate in street_list[i].coordinate:
                            street_list_coordinate = street_list_coordinate[1:-1].split(",")
                            # coor = (coor[0],coor[1])
                            #print(coor, street_list_coordinate, coor == street_list_coordinate, street_list[i].intersection_list == [])
                            if coor == street_list_coordinate and street_list[i].intersection_list == []:
                                V_pop_index.append(idx_V)
                
                for idx_V in V_pop_index:
                    V.pop(idx_V)

                for index in range(len(E)):
                    for V_idx in original_coor_Vindex:
                        if V_idx in E[index] and not index in original_coor_Eindex:
                            original_coor_Eindex.append(index)
                New_E = []
                for idx_E, element in enumerate(E):
                    if not idx_E in original_coor_Eindex:
                        New_E.append(E[idx_E])
                E = New_E
                if street_index != None:
                    street_list.pop(street_index)
                #gg()
                check_single_intersection()
                rewrite_E()
                return


        elif cmd == 'mod':
            pattern = r'^\(\s*-?\s*\d+\s*,\s*-?\s*\d+\s*\)$'
            for item in vetex_list:
                if not re.match(pattern, item):
                    print("Error: Wrong input coordinates format for: " + line_input, file=sys.stderr)
                    return
            original_coor_list = []
            original_coor_Vindex = []
            original_coor_Eindex = []
            original_intersection_list = []
            street_index = None
            existance = False
            for i in range(len(street_list)):
                if street_name == street_list[i].name:
                    street_index = i
                    existance = True
                    # delete old info in V and E
                    if len(V) != 0:
                        original_coor_list = street_list[i].coordinate
                        original_intersection_list = street_list[i].intersection_list
                        intersection_occurances_list = [] 
                        for coor in original_coor_list:
                            coor = coor[1:-1].split(",")
                            coor = [str(coor[0]), str(coor[1])]
                            for idx, value in V.items():
                                if value == coor:
                                    original_coor_Vindex.append(idx)
            if not existance:
                print("Error: Street doesn't exist and cannot be modified: " + street_name, file=sys.stderr)
                return
            else:
                intersection_occurances_list = [0] * len(original_intersection_list)
                for i in range(len(street_list)):
                    for coor in street_list[i].intersection_list:
                        for idx,intersection_coor in enumerate(original_intersection_list):
                            if intersection_coor == coor:
                                intersection_occurances_list[idx] += 1
                #print(original_intersection_list)
                #print(intersection_occurances_list)

                for idx, value in enumerate(intersection_occurances_list):
                    if value < 2:
                        for idx_V, coor in V.items():
                            if original_intersection_list[idx] == coor:
                                original_coor_Vindex.append(idx_V)
                ### missing intersection!!!!!
                for idx in original_coor_Vindex:
                    V.pop(idx)
                
                for index in range(len(E)):
                    for V_idx in original_coor_Vindex:
                        if V_idx in E[index] and not index in original_coor_Eindex:
                            original_coor_Eindex.append(index)
                New_E = []
                for idx_E, element in enumerate(E):
                    if not idx_E in original_coor_Eindex:
                        New_E.append(E[idx_E])
                E = New_E
                new_object = Street(street_name,vetex_list)
                if existance:
                    if street_index != None:
                        street_list[street_index] = new_object
                #gg()
                check_single_intersection()
                rewrite_E()
                return 
               
        else:
            print('Error: unknown command: ' + cmd, file=sys.stderr)
    return cmd, coordinate_list

# According from ag_intersect.py
def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)

def check_single_intersection():
    global street_list,E,V
    seen_intersection = []
    intersection_occurance = []
    single_intersection = []
    indexE_need_combine = []
    valueE_need_combine = []
    intersection_index_list = []
    for street in street_list:
        for intersection in street.intersection_list:
            if not intersection in seen_intersection:
                intersection = [float(intersection[0]), float(intersection[1])]
                intersection = ['{:.0f}'.format(coord) if coord.is_integer() else '{:.5f}'.format(coord) for coord in intersection]
                seen_intersection.append(intersection)
                intersection_occurance.append(1)
            else:
                index = seen_intersection.index(intersection)
                intersection_occurance[index] += 1
    for index, value in enumerate(intersection_occurance):
        if value < 2:
            single_intersection.append(seen_intersection[index])
    for idx, value in V.items():
        for intersection_coor in single_intersection:
            if value == intersection_coor:
                intersection_index_list.append(idx)
    # remove single intersection from V
    V = {key: value for key, value in V.items() if key not in intersection_index_list}
    for idx, value in enumerate(E):
        for index in intersection_index_list:
            #print(type(E[idx]),type(value))
            #value = value #tuple(,)
            #value = str(value)[1:-1].split(",") # list ['','']
            #print(value, index, intersection_index_list)
            #print(type(value[0]), type(index), value[0], index)
            if value[0] == index or value[1] == index:
                indexE_need_combine.append(idx)
                valueE_need_combine.append(value)
                for street in street_list:
                    for inter_idx, intersection in enumerate(street.intersection_list):
                        intersection = (float(intersection[0]),float(intersection[1]))
                        for single_inter in single_intersection:
                            #print(single_inter, single_inter[0], type(single_inter[0]))
                            single_inter = (float(single_inter[0]),float(single_inter[1]))
                            if intersection == single_inter:
                                street.intersection_list.pop(inter_idx)
    # remove single intersection from E:
    E = [E[i] for i in range(len(E)) if i not in indexE_need_combine]
    grouped_E = []
    for idx in range(0,len(valueE_need_combine)):
        value0 = valueE_need_combine[idx]
        for idx_1 in range(idx+1,len(valueE_need_combine)):
            value1 = valueE_need_combine[idx_1]
            if value0[0] == value1[0] or value0[1] == value1[0]:
                grouped_E.append((value0,value1))
    for pair in grouped_E:
        value0 = pair[0]
        value1 = pair[1]
        combined = None
        combined_inv = None
        if value0[0] == value1[0]:
            combined = (value0[1],value1[1])
            combined_inv = (value1[1],value0[1])
        elif value0[0] == value1[1]:
            combined = (value0[1],value1[0])
            combined_inv = (value1[0],value0[1])
        elif value0[1] == value1[0]:
            combined = (value0[0],value1[1])
            combined_inv = (value1[1],value0[0])
        else:
            combined = (value0[0],value1[0])
            combined_inv = (value1[0],value0[0])
        if not combined in E and not combined_inv in E and not combined == combined_inv:
            E.append(combined)
        else:
            idx_E = None
            for idx, value in enumerate(E):
                if value == combined or value == combined_inv:
                    idx_E = idx
            if idx_E in intersection_index_list:
                if idx_E != None:
                    E.pop(idx_E)

    # for idx in range(1,len(indexE_need_combine)):
    #     index0 = idx - 1
    #     index1 = idx
    #     valueE = valueE_need_combine[]

    

class point(object):
    """A point in a two dimensional space"""
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    # def getX(self):
    #     return self.x

    # def getY(self):
    #     return self.y
    
    def __repr__(self):
        return '(' + pp(self.x) + ', ' + pp(self.y) + ')'


class line(object):
    """A line between two points"""
    def __init__(self, src, dst):
        self.src = src
        self.dst = dst
        

    def __repr__(self):
        return '['+ str(self.src) + '-->' + str(self.dst) + ']'


def intersect (l1, l2):
    """Returns a point at which two lines intersect"""
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y

    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    try:
        xcoor =  xnum / xden
    except Exception:
        return 0

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    ycoor = ynum / yden

    # check if the intersect is outside of two edges
    if (xcoor < l1.src.x and xcoor < l1.dst.x) or (xcoor > l1.src.x and xcoor > l1.dst.x) or \
        (xcoor < l2.src.x and xcoor < l2.dst.x) or (xcoor > l2.src.x and xcoor > l2.dst.x) or \
        (ycoor < l1.src.y and ycoor < l1.dst.y) or (ycoor > l1.src.y and ycoor > l1.dst.y) or \
        (ycoor < l2.src.y and ycoor < l2.dst.y) or (ycoor > l2.src.y and ycoor > l2.dst.y):
        return 0
    else:
        return point(xcoor, ycoor)
    

def output_E():
    global E
    # Convert the list of tuples to a string in the desired format
    #print("Total E:", len(E))
    return 'E = {\n' + ',\n'.join([f"<{x},{y}>" for x, y in E]) + '\n}'

def output_V():
    global V
    # Convert the list of tuples to a string in the desired format
    formatted_V = "\n".join([f'{key}: ({float(value[0]):.2f}, {float(value[1]):.2f})' for key, value in V.items()])
    print("V ={\n" + formatted_V +"\n}")

def main():
    global street_list
    global V, E, V_index
    #, V_index
    #V_index = 1
    V = {}
    E = []
    V_index = 1
    # print(output_E())

    street_list = []
    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    line_counter = 0
    while True:
        line = sys.stdin.readline()
        line_counter += 1
        if line == "":
            break
        elif not line.strip():
            continue
            print("Error: Empty command line at line " + str(line_counter), file=sys.stderr)
        else:
            parseLine(line)

    sys.exit(0)


if __name__ == "__main__":
    main()
