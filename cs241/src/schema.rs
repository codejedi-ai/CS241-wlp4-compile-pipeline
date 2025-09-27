// src/schema.rs
use async_graphql::{Schema, Object, Context, Result};

pub struct QueryRoot;

#[Object]
impl QueryRoot {
    async fn hello(&self, ctx: &Context<'_>) -> Result<String> {
        Ok("Hello, world!".to_string())
    }
}

pub type MySchema = Schema<QueryRoot, async_graphql::EmptyMutation, async_graphql::EmptySubscription>;