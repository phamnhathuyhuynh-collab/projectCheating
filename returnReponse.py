from openai import OpenAI
import os
client = OpenAI(
    api_key="KEY_API của m",
    base_url="https://api.groq.com/openai/v1",
)

def groq_call(text):
    if text != "Do not execute this one":
        response = client.responses.create(
        model="openai/gpt-oss-20b", 
        input="Chào Gemini, bạn có thể trả lời A, B, C hoặc D cho câu hỏi {}, không cần giải thích".format(text))
        return response.output_text
    return "ignore this message"

if __name__ == "__main__":
    print(groq_call("Câu 1. Ted and Amy _____ for 24 years.A. have been marriedB. marriedC. were marriedD. has been married"))
