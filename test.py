import subprocess
import io
import wave

def play_audio(data):
    # ذخیره داده در یک فایل wave
    with wave.open('output.wav', 'wb') as wf:
        wf.setnchannels(1)  # یک کانال (مونو)
        wf.setsampwidth(2)  # عرض نمونه به بیت (2 بایت)
        wf.setframerate(44100)  # نرخ نمونه‌برداری 44100 هرتز
        wf.writeframes(data)

    # اجرای فرآیند پخش با استفاده از aplay
    subprocess.run(['aplay', 'output.wav'])

# تابعی برای شبیه‌سازی دریافت داده به صورت stream
def get_audio_stream():
    # این قسمت را با لاگیک خودتان جایگزین کنید که از یک منبع صدا بخوانید (مثلاً از میکروفن یا فایل)
    # اینجا یک مثال ساده از ورودی استاندارد استفاده شده است.
    for _ in range(10):  # تعداد فریم‌ها (برای تست)
        yield io.BytesIO(b'\x00\x80' * 512)  # داده‌های صدا (مثال: صدای سکوت)

# دریافت داده به صورت stream و پخش صدا
for audio_data in get_audio_stream():
    play_audio(audio_data.getvalue())

