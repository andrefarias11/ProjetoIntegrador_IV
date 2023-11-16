from flask import Flask, render_template, Response
from ultralytics import YOLO
from PIL import Image
import cv2

app = Flask(__name__)

def video_stream():
    # Load the YOLOv8 model
    model = YOLO('yolov8n.pt')

    # Open the video file
    video_path = "/home/avell/Videos/libraryBooksPeople01.mp4"
    cap = cv2.VideoCapture(0)

    # Loop through the video frames
    while cap.isOpened():
        # Read a frame from the video
        success, frame = cap.read()

        if success:
            # Run YOLOv8 inference on the frame
            results = model(frame)

            # Visualize the results on the frame
            annotated_frame = results[0].plot()

            print("resultados", results[0].verbose())

            # Display the annotated frame HERE
            #jpeg = cv2.imshow("YOLOv8 Inference", annotated_frame)
            jpeg = cv2.imencode(".jpg", annotated_frame)[1].tobytes()

            yield jpeg

            # Break the loop if 'q' is pressed
            if cv2.waitKey(1) & 0xFF == ord("q"):
                break
        else:
            # Break the loop if the end of the video is reached
            break

    # Release the video capture object and close the display window
    cap.release()
    cv2.destroyAllWindows()


@app.route('/')
def index():

    # Load a pretrained YOLOv8n mode
    model = YOLO('yolov8n.pt')

    # Run inference on 'bus.jpg'
    results = model('/home/avell/Pictures/outras/marlon-palestra.jpeg')  # results list

    # Show the results
    for r in results:
        im_array = r.plot()  # plot a BGR numpy array of predictions
        im = Image.fromarray(im_array[..., ::-1])  # RGB PIL image
        im.show()  # show image
        im.save('./static/results.jpg')  # save image



    return render_template('index.html')

@app.route('/video')
def video():

    return Response(video_stream(), mimetype="image/jpeg")

    #return render_template('index.html')


if __name__ == '__main__':
    app.run(debug=True)
