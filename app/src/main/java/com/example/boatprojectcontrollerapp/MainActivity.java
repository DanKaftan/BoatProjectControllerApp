package com.example.boatprojectcontrollerapp;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;

import android.os.AsyncTask;
import android.view.View;
import android.widget.Button;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {

    // ESP32 IP address
    private static final String ESP32_IP = "192.168.246.6";

    // ESP32 API endpoint
    private static final String ESP32_API = "http://" + ESP32_IP + "/motor";

    // Motor control buttons
    private Button btnMotorOn;
    private Button btnMotorOff;

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialize motor control buttons
        btnMotorOn = findViewById(R.id.btnMotorOn);
        btnMotorOff = findViewById(R.id.btnMotorOff);

        // Set click listeners for motor control buttons
        btnMotorOn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendMotorCommand("on");
            }
        });

        btnMotorOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendMotorCommand("off");
            }
        });
    }

    private void sendMotorCommand(String state) {
        String commandUrl = ESP32_API + "?state=" + state;

        // Execute the AsyncTask to send the motor command
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
