import random
import csv

# Parameters
order_count = 10000
instrument_list = ["Rose", "Lavender", "Lotus", "Tulip", "Orchid", "Sunflower", "Daisy", "Iris"]
valid_sides = [1, 2]
invalid_sides = [0, 3, "Canada", "7", 2014]
low_price = 0
high_price = 1000
valid_quantities = list(range(10, 1010, 10))  # Prices from 10 to 1000, multiple of 10
invalid_quantities = [0, 1001, 45.4, "junk", "123"]
error_probability = 0.05

# Error generators
def generate_error(value, value_type):
    error_type = random.choice(["instrument", "side", "quantity", "price"])
    if error_type == "instrument":
        return random.choice(instrument_list[5:])  # Invalid instruments
    elif error_type == "side":
        return random.choice(invalid_sides)  # Invalid sides
    elif error_type == "quantity":
        return random.choice(invalid_quantities)
    elif error_type == "price":
        return random.randint(1001, 100000)
    return value

# CSV Data Generation
with open('orders.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    
    # Generate rows
    for i in range(order_count):
        order_id = f"aa{0 + i}"
        instrument = random.choice(instrument_list[:5])
        side = random.choice(valid_sides)
        quantity = random.choice(valid_quantities)
        price = random.randint(low_price, high_price)
        writer.writerow([order_id, instrument, side, quantity, price])
        if random.random() < error_probability:
            random_index = random.randint(0, 4)
            if random_index == 0:
                instrument = generate_error(instrument, "instrument")
            elif random_index == 1:
                side = generate_error(side, "side")
            elif random_index == 2:
                quantity = generate_error(quantity, "quantity")
            elif random_index == 3:
                price = generate_error(price, "price")
            writer.writerow([order_id, instrument, side, quantity, price])

print("File generated successfully!")