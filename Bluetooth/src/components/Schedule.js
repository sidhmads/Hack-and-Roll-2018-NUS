import React, { Component } from 'react';
import { View, Text } from 'react-native';
import EasyBluetooth from 'easy-bluetooth-classic';
import DateTimePicker from 'react-native-modal-datetime-picker';
import 'moment-timezone';
import { Button, CardSection } from './common';

class Schedule extends Component {
  state = {
   isDateTimePickerVisible: false,
   day: '',
   months: '',
   year: '',
   hours: '',
   minutes: '',
   fullDate: ''
 };

 _showDateTimePicker = () => this.setState({ isDateTimePickerVisible: true });

 _hideDateTimePicker = () => this.setState({ isDateTimePickerVisible: false });

 _handleDatePicked = (datetime) => {
   console.log('A date has been picked: ', datetime);
   this.setState({
      day: datetime.getDate(),
      month: datetime.getMonth() + 1,
      year: datetime.getFullYear(),
      hours: datetime.getHours(),
      minutes: datetime.getMinutes()
    });
   this._hideDateTimePicker();
   this.setState({ fullDate: this.state.day +
      ':' + this.state.month +
      ':' + this.state.year +
      ':' + this.state.hours +
      ':' + this.state.minutes });
   console.log(this.state.fullDate);
   EasyBluetooth.write(`t ${this.state.fullDate};`);
   // EasyBluetooth.writeln(datetime)
   //  .then(() => {
   //    console.log('Writing...');
   //  });
 };

 // sentDateTime() {
 //      const dateToFormat = '1976-04-19T12:59-0500';
 //      return <Moment>{dateToFormat}</Moment>;
 // }

  render() {
    return (
      <View style={{ flex: 1 }}>
        <CardSection>
          <Button pressed={this._showDateTimePicker}>
            <Text>Show DatePicker</Text>
          </Button>
        </CardSection>
          <DateTimePicker
            isVisible={this.state.isDateTimePickerVisible}
            onConfirm={this._handleDatePicked}
            onCancel={this._hideDateTimePicker}
            mode='datetime'
          />
        </View>
    );
  }
}

export default Schedule;
