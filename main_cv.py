import csv
import sys
import cv2
import numpy as np


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


def draw_objects(image, objects, shift=(250, 250), scale=3):
    for obj in objects:
        x, y = int(obj[0] * scale + shift[0]), int(obj[1] * scale + shift[1])
        cv2.circle(image, (x, y), 5, (0, 255, 0), -1)


def main():
    data = parse_csv_data("DevelopmentData_modded.csv")
    kf = init_kalman_filter()
    kf = init_kalman_filter()

    for row in data:
        positions = [(row[f"FirstObjectDistance_X"], row[f"FirstObjectDistance_Y"]),
                     (row[f"SecondObjectDistance_X"], row[f"SecondObjectDistance_Y"]),
                     (row[f"ThirdObjectDistance_X"], row[f"ThirdObjectDistance_Y"]),
                     (row[f"FourthObjectDistance_X"], row[f"FourthObjectDistance_Y"])]

        positions = [pos for pos in positions if pos[0] != 0 and pos[1] != 0]

        image = np.zeros((800, 800, 3), dtype=np.uint8)

        # Apply the Kalman filter to the positions
        filtered_positions = []
        for pos in positions:
            kf.correct(np.array(pos, dtype=np.float32).reshape(2, 1))
            filtered_pos = kf.predict()
            filtered_positions.append((filtered_pos[0], filtered_pos[1]))

        draw_objects(image, filtered_positions)

        cv2.imshow("Objects", image)
        cv2.waitKey(100)

    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
