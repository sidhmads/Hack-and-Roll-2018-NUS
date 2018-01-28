import React from 'react';
import { View, Text, Image } from 'react-native';

const Weather = () => {
state = { rain: '1' };
  return (
    <View>
      if (this.state.rain === '1') {
          <Image
            style={{ width: 300, height: 300, alignSelf: 'center' }}
            source={require('../img/raining.jpg')}
          />
        }
    </View>
);
};

export default Weather;
