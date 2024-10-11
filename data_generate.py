import random
import csv

# Parameters
order_count = 10000
instrument_list = ["Rose", "Lavender", "Lotus", "Tulip", "Orchid", "Sunflower", "Daisy", "Iris"]
valid_sides = [1, 2]
valid_prices = list(range(10, 1010, 10))  # Prices from 10 to 1000, multiple of 10
error_probability = 0.05

# Error generators
def generate_error(value, value_type):
    error_type = random.choice(["missing", "invalid"])
    if error_type == "missing":
        return ""
    elif error_type == "invalid":
        if value_type == "side":
            return random.choice([0, 3, "X"])  # Invalid sides
        elif value_type == "quantity":
            return random.choice([-5, 1500, 19])  # Invalid quantities
        elif value_type == "price":
            return random.choice([-100, 55.5, 1200])  # Invalid prices
    return value

# CSV Data Generation
with open('orders.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    
    # Generate rows
    for i in range(order_count):
        # Client Order ID
        order_id = f"aa{13 + i}"
        
        # Instrument (mostly select from top 5)
        instrument = random.choice(instrument_list[:5]) if random.random() > error_probability else random.choice(instrument_list[5:])
        
        # Side (valid or invalid based on error probability)
        side = random.choice(valid_sides)
        if random.random() < error_probability:
            side = generate_error(side, "side")
        
        # Quantity (valid or invalid based on error probability)
        quantity = random.randint(1, 1000)
        if random.random() < error_probability:
            quantity = generate_error(quantity, "quantity")
        
        # Price (valid or invalid based on error probability)
        price = random.choice(valid_prices)
        if random.random() < error_probability:
            price = generate_error(price, "price")
        
        # Write to CSV
        writer.writerow([order_id, instrument, side, quantity, price])

print("File generated successfully!")
