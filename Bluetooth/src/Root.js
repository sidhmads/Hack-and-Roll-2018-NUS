import React, { Component } from 'react';
import EasyBluetooth from 'easy-bluetooth-classic';
import SplashScreen from 'react-native-splash-screen';
import Router from './components/Router';

class Root extends Component {

  componentWillMount() {
    EasyBluetooth.init({
        "uuid": '00001101-0000-1000-8000-00805f9b34fb',
        "deviceName": 'Bluetooth',
        "bufferSize": 1024,
        "characterDelimiter": '\n'
      })
      .then(() => {
        console.log('config done!');
      })
      .catch((ex) => {
        console.warn(ex);
      });
    }

    componentDidMount() {
      SplashScreen.hide();
  }
  render() {
    return (
      <Router />
    );
  }
}

export default Root;
