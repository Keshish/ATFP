import csv
import sys
import OpenGL.GL as gl
import OpenGL.GLUT as glut


def parse_csv_data(file_path):
    data = []
    with open(file_path, 'r') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            data.append(row)
    return data


def filter_data(data):
    return data


def display():
    gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)

    for obj in filtered_data:
        gl.glPushMatrix()

        # prepare 2d space
        gl.glTranslatef(float(obj['x']), float(obj['y']), 0.0)
        gl.glRotatef(float(obj['angle']), 0.0, 0.0, 1.0)
        gl.glScalef(float(obj['width']), float(obj['height']), 1.0)

        # draw object
        gl.glBegin(gl.GL_QUADS)
        gl.glColor3f(1.0, 0.0, 0.0)
        gl.glVertex2f(-0.5, -0.5)
        gl.glVertex2f(0.5, -0.5)
        gl.glVertex2f(0.5, 0.5)
        gl.glVertex2f(-0.5, 0.5)
        gl.glEnd()

        gl.glPopMatrix()

    glut.glutSwapBuffers()


def main():
    global filtered_data
    data = parse_csv_data("DevelopmentData_modded.csv")
    filtered_data = filter_data(data)

    glut.glutInit(sys.argv)
    glut.glutInitDisplayMode(glut.GLUT_DOUBLE | glut.GLUT_RGB | glut.GLUT_DEPTH)
    glut.glutCreateWindow("OpenGL Visualization")
    glut.glutDisplayFunc(display)

    gl.glEnable(gl.GL_DEPTH_TEST)
    gl.glEnable(gl.GL_LIGHTING)
    gl.glEnable(gl.GL_LIGHT0)
    gl.glEnable(gl.GL_COLOR_MATERIAL)

    glut.glutMainLoop()


if __name__ == "__main__":
    main()
