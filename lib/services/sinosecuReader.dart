import 'package:flutter/services.dart';

class sinosecuReader {
  static const MethodChannel _channel = MethodChannel('sinosecu_reader');

  static Future<int> initializeScanner() async {
    try {
      final int result = await _channel.invokeMethod('initialize');
      print('[Flutter] Scanner initialized. Result: $result');
      return result;
    } on PlatformException catch (e) {
      print('[Flutter] Failed to initialize scanner: ${e.message}');
      return -1;
    }
  }

  static Future<int> detectDocument() async {
    return await _channel.invokeMethod('detectDocument');
  }

  static Future<Map<String, dynamic>> autoProcess() async {
    final result = await _channel.invokeMethod('autoProcess');
    return Map<String, dynamic>.from(result);
  }

  static Future<void> free() async {
    await _channel.invokeMethod('free');
  }
}
