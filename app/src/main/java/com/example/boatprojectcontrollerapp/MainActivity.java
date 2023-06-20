package com.example.boatprojectcontrollerapp;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {

    // ESP32 IP address
    private static final String ESP32_IP = "192.168.99.6";

    // ESP32 API endpoint
    private static final String ESP32_API = "http://" + ESP32_IP + "/motor";

    // Motor control buttons
    private Button btnForward;
    private Button btnBackward;
    private SeekBar directionSeekBar;

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialize motor control buttons
        btnForward = findViewById(R.id.forwardButton);
        btnBackward = findViewById(R.id.backwardButton);
        directionSeekBar = findViewById(R.id.directionSeekBar);


        btnForward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN) {
                    System.out.println("Forward Button Pressed");
                    sendMotorCommand("forward");
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    System.out.println("Forward Button Released");
                    sendMotorCommand("stop");

                }

                return true;
            }
        });

        btnBackward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN) {
                    System.out.println("Backward Button Pressed");
                    sendMotorCommand("backward");
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    System.out.println("Backward Button Released");
                    sendMotorCommand("stop");

                }

                return true;
            }
        });

        // Update the SeekBar listener
        directionSeekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

                // Get servo degree from SeekBar progress
                //int servoDegree = progress;

                // Send servo degree to ESP32
                sendServoDegree(getServoDegree());
                System.out.println("servo degree of " + getServoDegree()  + " has been sent to esp32");
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                // Not used
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                // Not used
            }
        });

    }
    private int getServoDegree() {
        // Adjust this calculation based on your specific servo range and SeekBar setup
        return (int) (directionSeekBar.getProgress() * 1.8);
    }

    private void sendMotorCommand(String state) {
        String commandUrl = ESP32_API + "?state=" + state;

        // Execute the AsyncTask to send the motor command
        new SendCommandTask().execute(commandUrl);
    }

    private void sendServoDegree(int degree) {
        String commandUrl = ESP32_API + "?state=" + Integer.toString(degree);
        new SendCommandTask().execute(commandUrl);
    }

    private class SendCommandTask extends AsyncTask<String, Void, String> {

        @Override
        protected String doInBackground(String... urls) {
            String response = "";
            try {
                URL url = new URL(urls[0]);
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setRequestMethod("POST");
                conn.setConnectTimeout(5000);
                conn.setReadTimeout(5000);

                // Get the response from the ESP32
                BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                String line;
                while ((line = reader.readLine()) != null) {
                    response += line;
                }
                reader.close();
                conn.disconnect();

            } catch (Exception e) {
                e.printStackTrace();
                response = "Error: " + e.getMessage();
            }

            return response;
        }

        @Override
        protected void onPostExecute(String result) {
            // Process the response from the ESP32
            // Update the UI or perform any necessary actions
        }
    }
}



