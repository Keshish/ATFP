import csv
import cv2
import numpy as np
import math

def parse_csv_data(file_path):
    data = []
    with open(file_path, 'r') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            data.append(row)
    return data


def init_kalman_filter():
    kf = cv2.KalmanFilter(4, 2)
    kf.measurementMatrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0]], dtype=np.float32)
    kf.transitionMatrix = np.array([[1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 1, 0], [0, 0, 0, 1]], dtype=np.float32)
    kf.processNoiseCov = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]], dtype=np.float32) * 1e-2
    return kf


def draw_objects(image, vehicle_info, objects_info, shift=(250, 250), scale=3):
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.3
    font_thickness = 1

    # Draw vehicle
    vehicle_x, vehicle_y = int(vehicle_info["position"][0] * scale + shift[0]), int(vehicle_info["position"][1] * scale + shift[1])
    cv2.circle(image, (vehicle_x, vehicle_y), 5, (255, 0, 0), -1)
    vehicle_speed = vehicle_info["speed"]
    vehicle_text = f"Vehicle Speed: {vehicle_speed:.2f}"
    cv2.putText(image, vehicle_text, (vehicle_x - 50, vehicle_y - 30), font, font_scale, (255, 255, 255), font_thickness, cv2.LINE_AA)

    # Draw objects
    for i, obj in enumerate(objects_info):
        x, y = int(obj["position"][0] * scale + shift[0]), int(obj["position"][1] * scale + shift[1])
        cv2.circle(image, (x, y), 5, (0, 255, 0), -1)

        pos_x, pos_y = float(obj["position"][0]), float(obj["position"][1])
        speed_x, speed_y = float(obj["speed"][0]), float(obj["speed"][1])

        text = f"ID: {i+1} Pos: ({pos_x:.2f}, {pos_y:.2f}) Speed: ({speed_x:.2f}, {speed_y:.2f})"
        text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]
        text_x, text_y = x - text_size[0] // 2, y - 30

        cv2.putText(image, text, (text_x, text_y), font, font_scale, (255, 255, 255), font_thickness, cv2.LINE_AA)


def main():
    data = parse_csv_data("DevelopmentData_modded.csv")
    kf = init_kalman_filter()

    prev_timestamp = None
    prev_vehicle_speed = 0
    vehicle_position = np.array([0, 0], dtype=np.float32)

    for row in data:
        objects_info = [
            {
                "position": (row[f"FirstObjectDistance_X"], row[f"FirstObjectDistance_Y"]),
                "speed": (row[f"FirstObjectSpeed_X"], row[f"FirstObjectSpeed_Y"])
            },
            {
                "position": (row[f"SecondObjectDistance_X"], row[f"SecondObjectDistance_Y"]),
                "speed": (row[f"SecondObjectSpeed_X"], row[f"SecondObjectSpeed_Y"])
            },
            {
                "position": (row[f"ThirdObjectDistance_X"], row[f"ThirdObjectDistance_Y"]),
                "speed": (row[f"ThirdObjectSpeed_X"], row[f"ThirdObjectSpeed_Y"])
            },
            {
                "position": (row[f"FourthObjectDistance_X"], row[f"FourthObjectDistance_Y"]),
                "speed": (row[f"FourthObjectSpeed_X"], row[f"FourthObjectSpeed_Y"])
            }
        ]

        # Filter objects based on a threshold distance
        threshold_distance = 100
        objects_info = [obj for obj in objects_info if obj["position"][0] != 0 and obj["position"][1] != 0 and np.sqrt(float(obj["position"][0])**2 + float(obj["position"][1])**2) < threshold_distance]

        # Calculate vehicle speed
        timestamp = float(row["Timestamp"])
        yaw_rate = float(row["YawRate"])
        vehicle_speed = 0
        #print(f"Timestamp: {timestamp:.2f} Vehicle speed: {vehicle_speed:.2f} m/s Yaw rate: {yaw_rate:.2f} rad/s")
        if prev_timestamp is not None:
            dt = timestamp - prev_timestamp
            vehicle_speed = (vehicle_speed - prev_vehicle_speed) / dt
            print(f"Vehicle speed: {vehicle_speed:.2f} m/s")
            d_theta = yaw_rate * dt
            dx = vehicle_speed * math.cos(d_theta) * dt
            dy = vehicle_speed * math.sin(d_theta) * dt
            vehicle_position += np.array([dx, dy], dtype=np.float32)
            print(f"Vehicle position: {vehicle_position}")
        prev_timestamp = timestamp
        prev_vehicle_speed = vehicle_speed

        vehicle_info = {
            "position": tuple(vehicle_position),
            "speed": vehicle_speed
        }

        image = np.zeros((1000, 1000, 3), dtype=np.uint8)

        filtered_positions = []
        for obj in objects_info:
            kf.correct(np.array(obj["position"], dtype=np.float32).reshape(2, 1))
            filtered_pos = kf.predict()
            filtered_positions.append({
                "position": (filtered_pos[0] + float(obj["speed"][0]), filtered_pos[1] + float(obj["speed"][1])),
                "speed": obj["speed"]
            })

        draw_objects(image, vehicle_info, filtered_positions)
        cv2.imshow("Objects", image)
        cv2.waitKey(100)

    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()