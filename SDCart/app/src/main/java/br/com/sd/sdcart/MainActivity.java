package br.com.sd.sdcart;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class MainActivity extends AppCompatActivity implements MqttCallback {

    private String topic = "motor";
    private MqttClient client;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.buttonFrente).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    comando("1");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });

        findViewById(R.id.buttonEsq).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    comando("2");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });

        findViewById(R.id.buttonDir).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    comando("3");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });

        findViewById(R.id.buttonParar).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    comando("0");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });

        new Thread(){
            public void run(){
                String host = "tcp://m24.cloudmqtt.com:13278";
                String username = "zrupqoay";
                String password = "nloxRPMtkJIb";

                MqttConnectOptions conOpt = new MqttConnectOptions();
                conOpt.setCleanSession(true);
                conOpt.setUserName(username);
                conOpt.setPassword(password.toCharArray());

                try {
                    client = new MqttClient(host,  "controle", new MemoryPersistence());
                    client.setCallback(MainActivity.this);
                    client.connect(conOpt);
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        }.start();

    }



    @Override
    public void connectionLost(Throwable cause) {

    }

    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {

    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {

    }



    public void comando(String payload) throws MqttException {
        MqttMessage message = new MqttMessage(payload.getBytes());
        message.setQos(1);
        this.client.publish(this.topic, message); // Blocking publish
    }
}
