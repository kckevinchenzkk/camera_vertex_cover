import matplotlib.pyplot as plt

# Define the streets with their points
streets = {


    # "Oak Maple Way South": [(-16,16), (-13,-7), (4,-16)],
    # "Green Hill Rd West": [(8,10), (-8,-15), (3,17)],
    # "Dale Hill Blvd S": [(20,3), (16,5), (0,14), (-3,-18)],
    # "Queen Corrine Way North": [(-14,-16), (17,7), (-13,-7)],
    # "King Hill Ln South": [(1,-1), (-4,20), (13,-15), (9,6), (12,-6), (13,-12)],
    # "Green Elm Blvd East": [(8,0), (8,-13), (-5,-16), (-20,9), (-6,-19), (14,-14)],
    # "Elm Willow Cres S": [(-19,2), (6,5), (-11,20)],
        "Elm Victoria Dr East": [(14,3), (16,14), (8,8), (13,6), (0,0), (10,18), (2,1), (11,12), (18,17), (17,14), (18,9), (15,5), (17,10), (16,8),  ]
    # "Elm Green St E": [(6,16),, (1,-3), (0,-3), (-4,-8), (8,0), (15,3), (10,-1), (-1,-15)],
    # "Willow Maple St North": [(10,-3), (-15,19), (6,3), (14,17), (20,-6), (9,-8), (-4,-14), (-13,9), (-17,8)],
    # "Peach Sunset Blvd S": [(-9,-15), (-13,-12), (13,10), (14,-12), (-5,-8), (13,-9), (6,-6)],
    # "Mcknight Victoria Blvd N": [(-8,3), (-4,-7), (-10,-9)],
    # "Willow Maple St North": [(10,-3), (-15,19), (6,3), (14,17), (20,-6), (9,-8), (-4,-14), (-13,9), (-17,8)],
    # "Peach Sunset Blvd S": [(-9,-15), (-13,-12), (13,10), (14,-12), (-5,-8), (13,-9), (6,-6)],
    # "Mcknight Victoria Blvd N": [(-8,3), (-4,-7), (-10,-9)],
    # "Red Oak Ln South": [(20,-18), (-8,-3), (14,3), (7,-8), (12,-9), (17,14), (1,18), (0,1)],
    # "Red Stroud Way E": [(13,20), (-5,-5), (-1,-17)]
}

# Setup the plot
fig, ax = plt.subplots()

# Plot each street
for street, points in streets.items():
    # Unpack the points into x and y coordinates and plot them
    x, y = zip(*points)
    ax.plot(x, y, marker='o', label=street)

# Set the title and labels
ax.set_title('Street Map')
ax.set_xlabel('X Coordinate')
ax.set_ylabel('Y Coordinate')

# Set equal scaling
ax.axis('equal')

# Enable the legend
ax.legend()

# Show the plot
plt.show()
