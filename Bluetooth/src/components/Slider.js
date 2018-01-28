import React, { Component } from 'react';
import { View, Text, Slider } from 'react-native';
import EasyBluetooth from 'easy-bluetooth-classic';
import { Button, CardSection } from './common';

class Led extends Component {

  constructor(props) {
    super(props);
    this.state = {
      value: parseFloat(props.slider),
      updated: '0'
    };
  }

  change(value) {
    this.setState(() => {
      return {
        value: parseFloat(value),
        updated: '1'
      };
    });
}

showSliderVal() {
  if (this.state.updated === '1') {
    return <Text style={styles.text}>{String(this.state.value)}</Text>;
  }
  return <Text style={styles.text}>{String(this.state.slider)}</Text>;
}

  render() {
    const { value } = this.state;
    return (
    <View style={{ flex: 1 }}>
      <View style={styles.container}>
        <Slider
          step={1}
          maximumValue={90}
          onValueChange={this.change.bind(this)}
          value={value}
        />
        {this.showSliderVal()}
      </View>
      <CardSection>
        <Button
        pressed={
          () =>
          EasyBluetooth.write(`d ${this.state.value};`)
          .then(() => {
            console.log(`d ${value};`);
          })
        }
        >
        Send Slider value
        </Button>
      </CardSection>
    </View>
    );
  }
}

const styles = {
  container: {
    flex: 1,
    flexDirection: 'column',
    justifyContent: 'center',
  },
  text: {
    fontSize: 50,
    textAlign: 'center',
  },
};

export default Led;
