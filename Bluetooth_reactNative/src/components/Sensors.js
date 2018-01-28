import React, { Component } from 'react';
import { View, Text } from 'react-native';

class Sensors extends Component {

  render() {
    const { headerViewStyle, headerTextStyle, resultViewStyle, resultTextStyle } = styles;
    return (
      <View style={{ flex: 1, flexDirection: 'column' }}>

        <View>
          <View style={headerViewStyle}>
            <Text style={headerTextStyle}> Temperature (C) </Text>
          </View>
          <View style={resultViewStyle}>
            <Text style={resultTextStyle}> {this.props.temperature} </Text>
          </View>
        </View>

        <View>
          <View style={headerViewStyle}>
            <Text style={headerTextStyle}> Pressure (Pa) </Text>
          </View>
          <View style={resultViewStyle}>
            <Text style={resultTextStyle}> {this.props.pressure} </Text>
          </View>
        </View>

        <View>
          <View style={headerViewStyle}>
            <Text style={headerTextStyle}> Humidity (%RH) </Text>
          </View>
          <View style={resultViewStyle}>
            <Text style={resultTextStyle}> {this.props.humidity} </Text>
          </View>
        </View>

      </View>
    );
  }
}

const styles = {
  headerViewStyle: {
    alignItems: 'center',
    justifyContent: 'center',
    backgroundColor: '#F7FAF9',
  },
  headerTextStyle: {
    color: 'black',
    fontSize: 35,
    alignSelf: 'center',
    justifyContent: 'center'
  },
  resultViewStyle: {
    alignItems: 'center',
    justifyContent: 'center',
    backgroundColor: '#DCDCDC',
  },
  resultTextStyle: {
    alignSelf: 'center',
    color: 'black',
    backgroundColor: '#D3D3D3',
    fontSize: 28
  }
};
export default Sensors;
