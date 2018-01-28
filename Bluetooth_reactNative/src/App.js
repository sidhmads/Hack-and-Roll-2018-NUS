import React, { Component } from 'react';
import { View, Text } from 'react-native';
import EasyBluetooth from 'easy-bluetooth-classic';
import SplashScreen from 'react-native-splash-screen';
import { Button, CardSection } from './components/common';

class App extends Component {
  state = { status: '', textColor: 'red' };

  componentWillMount() {
    EasyBluetooth.init({
        "uuid": '00001101-0000-1000-8000-00805f9b34fb',
        "deviceName": 'Bluetooth',
        "bufferSize": 1024,
        "characterDelimiter": '\n'
      })
      .then(() => {
        console.log('config done!');
        this.setState({ status: 'please wait scanning for devices....' });
      })
      .catch((ex) => {
        console.warn(ex);
      });
      EasyBluetooth.startScan()
          .then((devices) => {
          devices.forEach(device => {
                 if (device.address === '98:D3:31:FC:2E:CE') {
                   EasyBluetooth.connect(device)
                   .then(() => {
                     console.log('Connected!');
                     this.setState({ status: 'connected and ready to use!', textColor: 'green' });
                     console.log(this.state.isConnected === true);
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

  componentDidMount() {
  // do stuff while splash screen is shown
    // After having done stuff (such as async tasks) hide the splash screen
    SplashScreen.hide();
}

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
    console.log(data);
  }

  onDeviceName(name) {
    console.log('onDeviceName');
    console.log(name);
  }

  showOptions() {
    return (
      <View>
        <Text style={{ color: this.state.textColor, fontSize: 22, backgroundColor: '#D3D3D3' }}> { this.state.status } </Text>
        <CardSection
        style={{ flex: 1, flexDirection: 'row', alignItems: 'center', justifyContent: 'center' }}
        >
         <Button
         pressed={
           () => EasyBluetooth.writeln('1')
            .then(() => {
              console.log('Writing...');
            })
            .catch((ex) => {
              console.warn(ex);
              console.log('err');
            })
         } color={{ backgroundColor: 'green' }}
         >
           Turn On
         </Button>
         <Button
         pressed={
           () => EasyBluetooth.writeln('0')
            .then(() => {
              console.log('Writing...');
            })
            .catch((ex) => {
              console.warn(ex);
              console.log('err');
            })
         }
        color={{ backgroundColor: 'red' }}
         >
           Turn Off
         </Button>
        </CardSection>
      </View>
    );
  }

  render() {
    return (
      <View>
      {this.showOptions()}
      </View>
    );
  }
}


export default App;
