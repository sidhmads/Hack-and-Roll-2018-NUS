import React, { Component } from 'react';
import { View } from 'react-native';
import EasyBluetooth from 'easy-bluetooth-classic';
import { Actions } from 'react-native-router-flux';

import { Button, CardSection } from './common';

class Bluetooth extends Component {
  state = { status: '', textColor: 'red', textOut: 'Scan and Connect', temperature: '', pressure: '', humidity: '', slider: '', raining: '' };


  onDeviceFound(device) {
    console.log('onDeviceFound');
    console.log(device);
  }

  onStatusChange(status) {
    console.log('onStatusChange');
    console.log(status);
  }

  onDataRead(data) {
    console.log('onDataRead');
    const values = data.split(';');
    for (var i = 0; i < values.length; i++) {
      if (values[i] === 'values') {
          this.setState({ temperature: data.split(';')[i + 1], pressure: data.split(';')[i + 2], humidity: data.split(';')[i + 3], slider: data.split(';')[i + 4], raining: data.split(';')[i + 5] });
          break;
      }
    }
    console.log(values);
    // this.setState({ temperature: data.split(';')[0], pressure: data.split(';')[1], humidity: data.split(';')[2], raining: data.split(';')[3] });
    Actions.refresh({ temperature: this.state.temperature, pressure: this.state.pressure, humidity: this.state.humidity, slider: this.state.slider, raining: this.state.raining });
  }

  onDeviceName(name) {
    console.log('onDeviceName');
    console.log(name);
  }

  scanAndConnect() {
    EasyBluetooth.startScan()
        .then((devices) => {
        devices.forEach(device => {
               if (device.address === '98:D3:31:FC:2E:CE') {
                 EasyBluetooth.connect(device)
                 .then(() => {
                   console.log('Connected!');
                   this.setState({ textOut: 'Connected!' });
                   Actions.connected(
                     { temperature: this.state.temperature,
                       pressure: this.state.pressure,
                       humidity: this.state.humidity,
                       slider: this.state.slider,
                       raining: this.state.raining
                    }
                   );
                 })
                 .catch((ex) => {
                   console.warn(ex);
                 });
               }
        });
     })
     .catch((ex) => {
          console.warn(ex);
        });
   this.onDeviceFoundEvent = EasyBluetooth.addOnDeviceFoundListener(this.onDeviceFound.bind(this));
   this.onStatusChangeEvent = EasyBluetooth.addOnStatusChangeListener(this.onStatusChange.bind(this));
   this.onDataReadEvent = EasyBluetooth.addOnDataReadListener(this.onDataRead.bind(this));
   this.onDeviceNameEvent = EasyBluetooth.addOnDeviceNameListener(this.onDeviceName.bind(this));
  }

  render() {
    return (
      <View>
        <CardSection>
          <Button
          pressed={() => {
                 this.scanAndConnect();
                 this.setState({ textOut: 'scanning....' });
              }}
          > {this.state.textOut} </Button>
        </CardSection>
      </View>
    );
  }
}

export default Bluetooth;
