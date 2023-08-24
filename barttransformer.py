from transformers import BartForConditionalGeneration, BartTokenizer
def translate():
    model = BartForConditionalGeneration.from_pretrained("facebook/bart-large", forced_bos_token_id=0)
    tok = BartTokenizer.from_pretrained("facebook/bart-large")
    example_english_phrase = "Hello there, how are you doing today? reply: <mask>"
    batch = tok(example_english_phrase, return_tensors="pt")
    generated_ids = model.generate(batch["input_ids"])
    print(tok.batch_decode(generated_ids, skip_special_tokens=True))